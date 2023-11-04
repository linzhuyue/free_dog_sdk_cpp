#ifndef _FDSC_HIGHSTATE_H_
#define _FDSC_HIGHSTATE_H_
#include <iostream>
#include <vector>
#include <array>
#include <fdsc_utils/enums.hpp>
#include <fdsc_utils/complex.hpp>
#include <fdsc_utils/common.hpp>

namespace FDSC{
    
    class highState {
        public:
            // all data in little endian 
            std::vector<uint8_t> head{0, 0};  //0-2
            uint8_t levelFlag = 0; //2
            uint8_t frameReserve = 0;//3
            std::vector<uint8_t> SN{0, 0, 0, 0, 0, 0, 0, 0}; //4-12
            std::vector<uint8_t> version{0, 0, 0, 0, 0, 0, 0, 0}; //12-20
            std::vector<uint8_t> bandWidth{0, 0};//20-22

            std::vector<float> imu_quaternion = std::vector<float>(4,0.0f); //4 flotas
            std::vector<float> imu_gyroscope = std::vector<float>(3,0.0f); 
            std::vector<float> imu_accelerometer = std::vector<float>(3,0.0f);
            std::vector<float> imu_rpy = std::vector<float>(3,0.0f);
            uint8_t temperature_imu = 0;
            Imu imu_ = Imu(imu_quaternion,imu_gyroscope,imu_accelerometer,imu_rpy,temperature_imu); // 22-75
            //motor state: 20 * 38
            int motormode = 0;
            float q = 0;
            float dq = 0;
            float ddq = 0;
            float tauEst = 0;
            float q_raw = 0;
            float dq_raw = 0;
            float ddq_raw = 0;            
            uint8_t motor_temperature = 0;
            std::vector<uint8_t> res_bms{0,0,0,0,0,0,0,0};
            std::vector<MotorState> motorState =std::vector<MotorState>(20, MotorState(motormode, q, dq, ddq, tauEst, q_raw, dq_raw, ddq_raw, motor_temperature, res_bms)); //
            uint8_t version_h = 0;
            uint8_t version_l = 0;
            uint8_t bms_status = 0;
            uint8_t SOC = 0;
            int32_t current = 0;
            uint16_t cycle = 0;
            std::vector<uint8_t> BQ_NTC{0,0} ;  //2Bytes
            std::vector<uint8_t> MCU_NTC{0,0};  //2Bytes
            // TODO here have problem with BMS cell vol don't care BMS data is zero in GO1 air
            std::vector<uint16_t> cell_vol = std::vector<uint16_t>(10,0); //10 int 
            BmsState bm_s= BmsState(version_h, version_l, bms_status, SOC, current, cycle, BQ_NTC, MCU_NTC, cell_vol); // 835-869
            
            std::vector<uint16_t> footForce = {0, 0, 0, 0}; //869-877. 2Bytes for every
            std::vector<uint16_t> footForceEst = {0, 0, 0, 0}; //877-885
            uint8_t Robotmode = 0; //885 //
            float progress = 0.0; //886-890
            uint8_t gaitType = 0; // 890
            float footRaiseHeight = 0.0; //891-895
            std::vector<float> position = {0.0, 0.0, 0.0};//895-907
            float bodyHeight = 0.0; //907-911
            std::vector<float> velocity = {0.0, 0.0,0.0}; //911-923
            float yawSpeed = 0.0;//923-927
            // not used in go1 air 
            std::vector<float> rangeObstacle = {0.0, 0.0, 0.0, 0.0}; //927-943
            std::vector<Cartesian> footPosition2Body = std::vector<Cartesian>(4,Cartesian(0.0,0.0,0.0)); //48 bytes 943-991
            std::vector<Cartesian> footSpeed2Body = std::vector<Cartesian>(4,Cartesian(0.0,0.0,0.0)); //48bytes 991-1039
            std::vector<uint8_t> wirelessRemote = std::vector<uint8_t>(40,0);//1039:1079
            std::vector<uint8_t> reserve = std::vector<uint8_t>(4,0);//1079-1083
            std::vector<uint8_t> crc = std::vector<uint8_t>(4,0);//1083-1087
            highState() {};

        void dataToBmsState(const std::vector<uint8_t>& data) {
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

        void dataToImu(const std::vector<uint8_t>& data) {

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

        void dataToMotorState(const std::vector<uint8_t>& data,int index) {
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

        inline  void parseData(const std::vector<uint8_t>& data) {
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


        };
} //END FDSC
#endif