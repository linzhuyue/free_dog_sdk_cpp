#include  <fdsc_utils/highState.hpp>

namespace FDSC
{
        void highState::dataToBmsState(const std::vector<uint8_t>& data) {
                version_h = data[0];
                version_l = data[1];
                bms_status = data[2];
                SOC = data[3];
                current = hex_to_int_i(data, 4, 8);
                cycle = hex_to_u16_i(data, 8, 10);
                BQ_NTC[0] =  data[10];
                BQ_NTC[1] =  data[11];
                MCU_NTC[0] = data[12];
                MCU_NTC[1] = data[13];

                for (int i = 0; i < 10; i++) {
                    cell_vol[i] = hex_to_u16_i(data, 13 + i * 2, 15 + i * 2);
                }
            }

        void highState::dataToImu(const std::vector<uint8_t>& data) {

                for (int i = 0; i < 4; i++) {
                    imu_quaternion[i] = hex_to_float_i(data, i * 4, (i + 1) * 4);
                }

                for (int i = 0; i < 3; i++) {
                    imu_gyroscope[i] = hex_to_float_i(data, 16 + i * 4, 16 + (i + 1) * 4);
                    imu_accelerometer[i] = hex_to_float_i(data, 28 + i * 4, 28 + (i + 1) * 4);
                    imu_rpy[i] = hex_to_float_i(data, 40 + i * 4, 40 + (i + 1) * 4);
                }
                temperature_imu = data[52];
            }

        void highState::dataToMotorState(const std::vector<uint8_t>& data,int index) {
                motormode =int(data[0]);
                q = hex_to_float_i(data,   1,   5);
                dq = hex_to_float_i(data,   5,   9);
                ddq = hex_to_float_i(data,   9,   13);
                tauEst = hex_to_float_i(data,   13,   17);
                q_raw = hex_to_float_i(data,   17,   21);
                dq_raw = hex_to_float_i(data,   21,   25);
                ddq_raw = hex_to_float_i(data,   25,   29);
                motor_temperature = data[29];
                std::copy(data.begin()+30,data.end(),res_bms.begin());
                motorState[index].set_data(motormode, q, dq, ddq, tauEst, q_raw, dq_raw, ddq_raw, motor_temperature, res_bms);
            }

        void highState::parseData(const std::vector<uint8_t>& data) {
                // data length is 1087
                std::copy(data.begin(),data.begin()+2, head.begin()); //0-2
                levelFlag = data[2];
                frameReserve = data[3];
                std::copy(data.begin()+4,data.begin()+12, SN.begin()); //
                std::copy(data.begin()+12,data.begin()+20, version.begin()); //
                bandWidth[0] = data[20];
                bandWidth[1] = data[21];
                
                std::vector<uint8_t> imu_data(data.begin()+22,data.begin()+75);
                dataToImu(imu_data); 
                
                for (int i = 0; i < 20; i++) {
                    std::vector<uint8_t> motor_tmp_data(data.begin()+75 + i*38,data.begin()+38+75 + i*38);
                    dataToMotorState(motor_tmp_data,i);
                }
                

                std::vector<uint8_t> bms_data(data.begin()+835,data.begin()+869);
                dataToBmsState(bms_data);
                // we do not have this in go 1 air
                for (int i = 0; i < 4; i++) {
                    footForce[i] = hex_to_u16_i(data, 869 + i * 2, 871 + i * 2);
                    footForceEst[i] = hex_to_u16_i(data, 877 + i * 2, 879 + i * 2);
                }
                
                Robotmode = data[885];
                progress = hex_to_float_i(data, 886, 890);
                gaitType = data[890];
                footRaiseHeight = hex_to_float_i(data, 891, 895);
                
                position[0] = hex_to_float_i(data, 895, 899);
                position[1] = hex_to_float_i(data, 899, 903);
                position[2] = hex_to_float_i(data, 903, 907);
                bodyHeight  = hex_to_float_i(data, 907, 911);
                
                velocity[0] = hex_to_float_i(data, 911, 915);
                velocity[1] = hex_to_float_i(data, 915, 919);
                velocity[2] = hex_to_float_i(data, 919, 923);
                yawSpeed    = hex_to_float_i(data, 923, 927);
                // // we have ultrasound in go 1 air
                for (int i = 0; i < 4; i++) {
                    rangeObstacle[i] = hex_to_float_i(data, 927 + i * 4, 931 + i * 4);
                    footPosition2Body[i] = Cartesian(hex_to_float_i(data, 943 + i * 12, 947 + i * 12),
                                                     hex_to_float_i(data, 947 + i * 12, 951 + i * 12),
                                                     hex_to_float_i(data, 951 + i * 12, 955 + i * 12));
                    footSpeed2Body[i] =    Cartesian(hex_to_float_i(data, 991 + i * 12, 995 + i * 12),
                                                     hex_to_float_i(data, 995 + i * 12, 999 + i * 12),
                                                     hex_to_float_i(data, 999 + i * 12, 1003 + i * 12));
                }
                std::copy(data.begin()+1039,data.begin()+1079, wirelessRemote.begin()); //
                std::copy(data.begin()+1079,data.begin()+1083, reserve.begin()); //
                std::copy(data.begin()+1083,data.end(), crc.begin()); //
            }


        
}