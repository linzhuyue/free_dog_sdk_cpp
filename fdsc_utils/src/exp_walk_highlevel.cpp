#include <iostream>
#include <vector>
#include <array>
#include <cstring>
#include <thread>
#include <fdsc_utils/highCmd.hpp>
#include <fdsc_utils/highState.hpp>
// #include <fdsc_utils/unitreeConnect.hpp>
#include <fdsc_utils/unitreeConnectBoost.hpp> //more fast and safe than the above one
#include <fdsc_utils/complex.hpp>
#include <fdsc_utils/color.hpp>
void show_joint_info(const std::vector<FDSC::MotorState> & mobj)
{
    for (int i = 0; i < 12; i++)
    {
        std::cout<<"Joint: "<<i<<"   Motor Mode: "<< mobj[i].mode <<" Motor NTC: "<<mobj[i].temperature<<" ";
        std::cout<<" q: "<<mobj[i].q<<" dq: "<<mobj[i].dq<<" ddq: "<<mobj[i].ddq<<" TauEst: "<<mobj[i].tauEst<<std::endl;
    }

}
int main() {
    std::string settings = "HIGH_WIFI_DEFAULTS";//"SIM_DEFAULTS";//HIGH_WIFI_DEFAULTS
    FDSC::UnitreeConnection conn(settings);
    conn.startRecv();
    FDSC::highCmd hcmd;
    FDSC::highState hstate;

    // Send an empty command to tell the dog the receive port and initialize the connection
    std::vector<uint8_t> cmd_bytes = hcmd.buildCmd(false);
    conn.send(cmd_bytes);
    // std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Some time to collect packets

    int motiontime = 0;
    int max_iter = 240;
    bool show_info = false;
    while (true) {
        std::vector<std::vector<uint8_t>> dataall = conn.getData();
        if (dataall.size()!=0)
        {
            std::cout<<"reveive pkg len: "<<dataall.size()<<" Motiontime: "<<motiontime<<std::endl;
            if(motiontime % 100 == 0 && show_info)
            {
                std::cout<<"reveive pkg len: "<<dataall.size()<<" Motiontime: "<<motiontime<<std::endl;
                std::vector<uint8_t> data = dataall.at(dataall.size()-1);
                std::cout<<"loop: "<<motiontime<<" datalen: " <<std::dec<<data.size()<<std::endl;
                std::cout << "=============================" << std::endl;
                hstate.parseData(data);
                std::cout << SetForeGRN <<"------------------HardSoft Version info: -------------------" << std::endl;
                std::cout << "SN: " << "\t";     FDSC::byte_print(hstate.SN,false);std::cout << FDSC::decode_sn(hstate.SN)<< std::endl;
                std::cout << "Version: " << "\t";FDSC::byte_print(hstate.version,false);std::cout<< FDSC::decode_version(hstate.version) << std::endl;
                std::cout << SetForeGRN <<"------------------IMU info: -------------------" << std::endl;
                std::cout << "IMU Quaternion: ";    FDSC::pretty_show_vector(hstate.imu_quaternion);std::cout << std::endl;
                std::cout << "IMU Gyroscope: ";     FDSC::pretty_show_vector(hstate.imu_gyroscope)  ;std::cout<< std::endl;
                std::cout << "IMU Accelerometer: "; FDSC::pretty_show_vector(hstate.imu_accelerometer);std::cout << std::endl;
                std::cout << "IMU RPY: ";           FDSC::pretty_show_vector(hstate.imu_rpy) ;
                std::cout<<  "IMU Temperature: " << float(hstate.temperature_imu) << std::endl;
                // std::vector<uint8_t> imu_data(data.begin()+22,data.begin()+75);
                // FDSC::show_byte_data(data,8);
                // std::cout<<std::endl;
                std::cout<<SetForeGRN << "------------------Joint info: -------------------" << std::endl;
                show_joint_info(hstate.motorstate);
                // BMS do not have any data from UDP data
                std::cout<<SetForeRED << "------------------BMS info(No data): -------------------" << std::endl;
                std::cout << "SOC: " << int(hstate.SOC) << std::endl;
                // std::cout << "Overall Voltage: " << FDSC::getVoltage(hstate.cell_vol) << "mV" << std::endl;
                std::cout << "Current: " << hstate.current << "mA" << std::endl;
                std::cout << "Cycles: " << hstate.cycle << std::endl;
                std::cout << "Temps BQ: " << hstate.BQ_NTC[0] << "°C, " << hstate.BQ_NTC[1] << "°C" << std::endl;
                std::cout << "Temps MCU: " << hstate.MCU_NTC[0] << "°C, " << hstate.MCU_NTC[1] << "°C" << std::endl;
                std::cout<<SetForeRED << "------------------Foot Force info(No data In Go1 Air): -------------------" << std::endl;
                std::cout<<"Footforce: ";
                for (auto data_f : hstate.footForce)
                {
                    std::cout<<float(data_f)<<" ";
                }
                std::cout<<std::endl;
                std::cout<<"FootforceEst: ";
                for (auto data_f : hstate.footForceEst)
                {
                    std::cout<<float(data_f)<<" ";
                }
                std::cout<<std::endl;

                std::cout<<SetForeGRN << "------------------Motion Info: -------------------" << std::endl;
                std::cout<<"Robotmode: "<<int(hstate.Robotmode)<<std::endl;// 
                std::cout<<"Progress: " <<hstate.progress<<std::endl;// TODO do not know this?
                std::cout<<"GaitType: "<<int(hstate.gaitType) <<std::endl;
                std::cout<<"footRaiseHeight: "<<hstate.footRaiseHeight <<std::endl;
                std::cout<<"BodyHeight: "<<hstate.bodyHeight <<std::endl;
                std::cout<<"Body Pos: [x,y,z] "<<hstate.position[0]<<" "<< hstate.position[1]<<" "<<hstate.position[2]<<std::endl;
                std::cout<<"Body Vel: [vx,vy,vz] "<<hstate.velocity[0]<<" "<< hstate.velocity[1]<<" "<<hstate.velocity[2]<<std::endl;
                std::cout<<"YawSpeed: "<<hstate.yawSpeed <<std::endl;
                std::cout<<"rangeObs: [o1,o2,o3,o4] "<<hstate.rangeObstacle[0]<<" "<< hstate.rangeObstacle[1]<<" "<<hstate.rangeObstacle[2]<<" "<<hstate.rangeObstacle[3]<<std::endl;
                for (int i=0;i<4;i++)
                {
                    std::cout<<"footpose2body: "<<"Leg: "<<i<<" "<<hstate.footPosition2Body[i].x<<" "<<hstate.footPosition2Body[i].y<<" "<<hstate.footPosition2Body[i].z<<std::endl;
                    std::cout<<"footspeed2body: "<<"Leg: "<<i<<" "<<hstate.footSpeed2Body[i].x<<" "<<hstate.footSpeed2Body[i].y<<" "<<hstate.footSpeed2Body[i].z<<std::endl;
                }
                std::cout << "=============================" << std::endl;
            }
            if (motiontime > 0 && motiontime < 10) {
                hcmd.robotmode_ = FDSC::ROBOTModeHigh::FORCE_STAND;
                hcmd.euler.at(0) = -0.3;
                hcmd.euler.at(1) = 0;
                hcmd.euler.at(2) = 0;
            }

            if (motiontime > 10 && motiontime < 20) {
                hcmd.robotmode_ = FDSC::ROBOTModeHigh::FORCE_STAND;
                hcmd.euler.at(0) = 0.3;
                hcmd.euler.at(1) = 0;
                hcmd.euler.at(2) = 0;
            }

            if (motiontime > 20 && motiontime < 30) {
                hcmd.robotmode_ = FDSC::ROBOTModeHigh::FORCE_STAND;
                hcmd.euler.at(0) =  0.0;
                hcmd.euler.at(1) = -0.2;
                hcmd.euler.at(2) = 0;
            }

            if (motiontime > 30 && motiontime < 40) {
                hcmd.robotmode_ = FDSC::ROBOTModeHigh::FORCE_STAND;
                hcmd.euler.at(0) = 0.;
                hcmd.euler.at(1) = 0.2;
                hcmd.euler.at(2) = 0;
            }

            if (motiontime > 40 && motiontime < 50) {
                hcmd.robotmode_ = FDSC::ROBOTModeHigh::FORCE_STAND;
                hcmd.euler.at(0) = 0.0;
                hcmd.euler.at(1) = 0;
                hcmd.euler.at(2) = -0.2;
            }

            if (motiontime > 50 && motiontime < 60) {
                hcmd.robotmode_ = FDSC::ROBOTModeHigh::FORCE_STAND;
                hcmd.euler.at(0) = 0.0;
                hcmd.euler.at(1) = 0;
                hcmd.euler.at(2) = 0.2;
            }

            if (motiontime > 60 && motiontime < 70) {
                hcmd.robotmode_ = FDSC::ROBOTModeHigh::FORCE_STAND;
                hcmd.bodyHeight = -0.2; // TODO should set to negtive 
            }

            if (motiontime > 70 && motiontime < 80) {
                hcmd.robotmode_ = FDSC::ROBOTModeHigh::FORCE_STAND;
                hcmd.bodyHeight = -0.1;
            }

            if (motiontime > 80 && motiontime < 90) {
                hcmd.robotmode_ = FDSC::ROBOTModeHigh::FORCE_STAND;
                hcmd.bodyHeight = -0.15;
            }

            if (motiontime > 90 && motiontime < 110) {
                hcmd.robotmode_ = FDSC::ROBOTModeHigh::STAND_DOWN;
            }

            if (motiontime > 110 && motiontime < 130) {
                hcmd.robotmode_ = FDSC::ROBOTModeHigh::STAND_UP;
            }

            if (motiontime > 130 && motiontime < 140) {
                hcmd.robotmode_ = FDSC::ROBOTModeHigh::IDLE;
            }
            if (motiontime > 140 && motiontime < 180) {
                hcmd.robotmode_ = FDSC::ROBOTModeHigh::VEL_WALK;
                hcmd.gaitType = FDSC::GaitType::TROT;
                hcmd.velocity.at(0) = 0.2;
                hcmd.velocity.at(1) = 0.0;
                hcmd.yawSpeed = 1.0;
                hcmd.footRaiseHeight = 0.13;
            }

            if (motiontime > 180 && motiontime < 200) {
                hcmd.robotmode_ = FDSC::ROBOTModeHigh::IDLE;
                hcmd.velocity.at(0) = 0.0;
                hcmd.velocity.at(1) = 0.0;
            }

            if (motiontime > 200 && motiontime < 240) {
                hcmd.robotmode_ = FDSC::ROBOTModeHigh::VEL_WALK;
                hcmd.gaitType = FDSC::GaitType::TROT;
                hcmd.velocity.at(0) = 0.0;
                hcmd.velocity.at(1) = 0.0;
                hcmd.bodyHeight = -0.1;
            }
            cmd_bytes = hcmd.buildCmd(false);
            conn.send(cmd_bytes);
            if (motiontime > max_iter) {
                hcmd.robotmode_= FDSC::ROBOTModeHigh::DAMPING;
                cmd_bytes = hcmd.buildCmd(false);
                conn.send(cmd_bytes);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            ++motiontime;
        }

    }

    return 0;
}
