#include <fdsc_utils/free_dog_sdk_h.hpp>


void show_joint_info(const std::vector<FDSC::MotorState> & mobj)
{
    for (int i = 0; i < 12; i++)
    {
        std::cout<<"Joint: "<<i<<"   Motor Mode: "<< mobj[i].mode <<" Motor NTC: "<<mobj[i].temperature<<" ";
        std::cout<<" q: "<<mobj[i].q<<" dq: "<<mobj[i].dq<<" ddq: "<<mobj[i].ddq<<" TauEst: "<<mobj[i].tauEst<<std::endl;
    }

}
void show_info(const FDSC::lowState & lstate)
{

        std::cout << "=============================" << std::endl;
        std::cout << SetForeGRN <<"------------------HardSoft Version info: -------------------" << std::endl;
        std::cout << "SN: " << "\t";     FDSC::byte_print(lstate.SN,false);std::cout << FDSC::decode_sn(lstate.SN)<< std::endl;
        std::cout << "Version: " << "\t";FDSC::byte_print(lstate.version,false);std::cout<< FDSC::decode_version(lstate.version) << std::endl;
        std::cout << "Bandwidth: " << "\t"<<FDSC::hex_to_kp_kd(lstate.bandWidth)<< std::endl;
        std::cout << SetForeGRN <<"------------------IMU info: -------------------" << std::endl;
        std::cout << "IMU Quaternion: ";    FDSC::pretty_show_vector(lstate.imu_quaternion);std::cout << std::endl;
        std::cout << "IMU Gyroscope: ";     FDSC::pretty_show_vector(lstate.imu_gyroscope)  ;std::cout<< std::endl;
        std::cout << "IMU Accelerometer: "; FDSC::pretty_show_vector(lstate.imu_accelerometer);std::cout << std::endl;
        std::cout << "IMU RPY: ";           FDSC::pretty_show_vector(lstate.imu_rpy) ;
        std::cout<<  "IMU Temperature: " << float(lstate.temperature_imu) << std::endl;
        // std::vector<uint8_t> imu_data(data.begin()+22,data.begin()+75);
        // FDSC::show_byte_data(data,8);
        // std::cout<<std::endl;
        std::cout<<SetForeGRN << "------------------Joint info: -------------------" << std::endl;
        show_joint_info(lstate.motorState);
        // BMS do not have any data from UDP data
        std::cout<<SetForeRED << "------------------BMS info(No data): -------------------" << std::endl;
        std::cout << "SOC: " << int(lstate.SOC) << std::endl;
        // std::cout << "Overall Voltage: " << FDSC::getVoltage(lstate.cell_vol) << "mV" << std::endl;
        std::cout << "Current: " << lstate.current << "mA" << std::endl;
        std::cout << "Cycles: " << lstate.cycle << std::endl;
        std::cout << "Temps BQ: " << lstate.BQ_NTC[0] << "°C, " << lstate.BQ_NTC[1] << "°C" << std::endl;
        std::cout << "Temps MCU: " << lstate.MCU_NTC[0] << "°C, " << lstate.MCU_NTC[1] << "°C" << std::endl;
        std::cout<<SetForeRED << "------------------Foot Force info(No data In Go1 Air): -------------------" << std::endl;
        std::cout<<"Footforce: ";
        for (auto data_f : lstate.footForce)
        {
            std::cout<<float(data_f)<<" ";
        }
        std::cout<<std::endl;
        std::cout<<"FootforceEst: ";
        for (auto data_f : lstate.footForceEst)
        {
            std::cout<<float(data_f)<<" ";
        }
        std::cout << "=============================" << std::endl;
        
}

int main() {
    std::string settings = "LOW_WIRED_DEFAULTS";//"SIM_DEFAULTS";//HIGH_WIFI_DEFAULTS
    FDSC::UnitreeConnection conn(settings);
    conn.startRecv();


    FDSC::lowCmd lcmd;
    FDSC::lowState lstate;
    FDSC::MotorCmdArray mCmdArr;
    // Send an empty command to tell the dog the receive port and initialize the connection
    std::vector<uint8_t> cmd_bytes = lcmd.buildCmd(false);

    conn.send(cmd_bytes);
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Some time to collect packets

    std::vector<float> qInit(3, 0.0);
    std::vector<float> qDes(3, 0.0);
    std::vector<float> Kp(3, 0.0);
    std::vector<float> Kd(3, 0.0);
    std::vector<float> sin_mid_q{0.0, 1.2, -2.0};
    float dt = 0.002;
    int sin_count = 0;
    int rate_count = 0;

   
    int motiontime = 0;
    int max_iter = 240;
    bool sim = false;
    FDSC::show_in_lowcmd();
    bool show_data = false;
    while (true) {
       
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        motiontime++;
        std::vector<std::vector<uint8_t>> dataall;// = conn.getData();
        conn.getData(dataall);
        if (dataall.size()!=0)
        {
            std::vector<uint8_t> data = dataall.at(dataall.size()-1);
            lstate.parseData(data);
            if (show_data && motiontime % 100 ==0)
            {
                std::cout<<"reveive pkg len: "<<dataall.size()<<std::endl;
                std::cout<<"loop: "<<motiontime<<" datalen: " <<std::dec<<data.size()<<std::endl;
                show_info(lstate);
            }
            if (motiontime > 0) {
                if (motiontime < 10) {
                    qInit[0] = lstate.motorState[FDSC::jointMapping["FR_0"]].q;
                    qInit[1] = lstate.motorState[FDSC::jointMapping["FR_1"]].q;
                    qInit[2] = lstate.motorState[FDSC::jointMapping["FR_2"]].q;
                }

                if (motiontime > 10 && motiontime < 400) {
                    rate_count++;
                    double rate = rate_count / 200.0;
                    Kp = {5, 5, 5};
                    Kd = {1, 1, 1};
                    qDes[0] = FDSC::jointLinearInterpolation(qInit[0], sin_mid_q[0], rate);
                    qDes[1] = FDSC::jointLinearInterpolation(qInit[1], sin_mid_q[1], rate);
                    qDes[2] = FDSC::jointLinearInterpolation(qInit[2], sin_mid_q[2], rate);
                    
                }
                double freq_Hz = 1.0;
                double freq_rad = freq_Hz * 2 * M_PI;
                double t = dt * sin_count;

                if (motiontime >= 400) {
                    sin_count++;
                    double sin_joint1 = 0.6 * sin(t * freq_rad);
                    double sin_joint2 = -0.9 * sin(t * freq_rad);
                    qDes[0] = sin_mid_q[0];
                    qDes[1] = sin_mid_q[1] + sin_joint1;
                    qDes[2] = sin_mid_q[2] + sin_joint2;
                }
                std::vector<float> FR0_joint{qDes[0], 0.0f,0.0f, Kp[0],Kd[0]};
                mCmdArr.setMotorCmd("FR_0", FDSC::MotorModeLow::Servo, FR0_joint);
                std::vector<float> FR1_joint{qDes[1], 0.0f,0.0f, Kp[1],Kd[1]};
                mCmdArr.setMotorCmd("FR_1", FDSC::MotorModeLow::Servo, FR1_joint);
                std::vector<float> FR2_joint{qDes[2], 0.0f,0.0f, Kp[2],Kd[2]};
                mCmdArr.setMotorCmd("FR_2", FDSC::MotorModeLow::Servo, FR2_joint);
                lcmd.motorCmd = mCmdArr;
                // std::cout<<SetForeMAG<<"Motiondata: "<<motiontime<<" qdes2 "<<qDes[2]<<" real_q: "<<lstate.motorState[FDSC::jointMapping["FR_2"]].q<<std::endl;
                std::vector<uint8_t> cmdBytes = lcmd.buildCmd(false);
                conn.send(cmdBytes);
            }
        }else{
            std::cout<<"out:motiontime: "<<motiontime<<" datasize: "<<dataall.size()<<std::endl;
        }
    
    }

    return 0;
}
