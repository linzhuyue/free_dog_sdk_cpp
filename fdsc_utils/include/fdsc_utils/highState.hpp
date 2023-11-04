#ifndef _FDSC_HIGHSTATE_H_
#define _FDSC_HIGHSTATE_H_
#include <iostream>
#include <vector>
#include <array>
#include <fdsc_utils/complex.hpp>
#include <fdsc_utils/common.hpp>

namespace FDSC{
    
    class highState {
        public:
            highState() {};
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
            
            void dataToBmsState(const std::vector<uint8_t>& data);
            void dataToImu(const std::vector<uint8_t>& data);
            void dataToMotorState(const std::vector<uint8_t>& data,int index);
            void parseData(const std::vector<uint8_t>& data) ;
    };
} //END FDSC
#endif