#ifndef _FDSC_LOWSTATE_H_
#define _FDSC_LOWSTATE_H_

#include <iostream>
#include <vector>
#include <array>

#include <fdsc_utils/complex.hpp>
#include <fdsc_utils/common.hpp>

namespace FDSC
{
    class lowState
    {
        public:
            // All data in little-endian, data length 807
            std::vector<uint8_t> head{0, 0}; // 0-2
            uint8_t levelFlag = 0;          // 2
            uint8_t frameReserve = 0;       // 3
            std::vector<uint8_t> SN{0, 0, 0, 0, 0, 0, 0, 0};      // 4-12
            std::vector<uint8_t> version{0, 0, 0, 0, 0, 0, 0, 0}; // 12-20
            std::vector<uint8_t> bandWidth{0, 0};                  // 20-22

            std::vector<float> imu_quaternion = std::vector<float>(4, 0.0f); // 4 floats
            std::vector<float> imu_gyroscope = std::vector<float>(3, 0.0f);
            std::vector<float> imu_accelerometer = std::vector<float>(3, 0.0f);
            std::vector<float> imu_rpy = std::vector<float>(3, 0.0f);
            uint8_t temperature_imu = 0;
            // Motor state: 20 * 32 in lowstate
            int motormode = 0;
            float q = 0;
            float dq = 0;
            float ddq = 0;
            float tauEst = 0;
            float q_raw = 0;
            float dq_raw = 0;
            float ddq_raw = 0;
            uint8_t motor_temperature = 0;
            std::vector<uint8_t> res_bms{0, 0, 0, 0, 0, 0, 0, 0};
            std::vector<MotorState> motorState = std::vector<MotorState>(20, MotorState(motormode, q, dq, ddq, tauEst, q_raw, dq_raw, ddq_raw, motor_temperature, res_bms));
            uint8_t version_h = 0;
            uint8_t version_l = 0;
            uint8_t bms_status = 0;
            uint8_t SOC = 0;
            int32_t current = 0;
            uint16_t cycle = 0;
            std::vector<uint8_t> BQ_NTC{0, 0}; // 2 bytes
            std::vector<uint8_t> MCU_NTC{0, 0}; // 2 bytes
            // TODO here have a problem with BMS cell vol, you don't care BMS data is zero in GO1 air
            std::vector<uint16_t> cell_vol = std::vector<uint16_t>(10, 0); // 10 int
            BmsState bm_s = BmsState(version_h, version_l, bms_status, SOC, current, cycle, BQ_NTC, MCU_NTC, cell_vol); // 715-739
            std::vector<uint16_t> footForce = {0, 0, 0, 0};  // 739-747. 2 bytes for every
            std::vector<uint16_t> footForceEst = {0, 0, 0, 0}; // 747-755
            std::vector<uint8_t> robotlowmode = std::vector<uint8_t>(4, 0); // TODO what ? 755-759
            xRockerBtnDataStruct wirelessdata;
            std::vector<uint8_t> wirelessRemote = std::vector<uint8_t>(40, 0); // 759-799
            std::vector<uint8_t> reserve = std::vector<uint8_t>(4, 0);           // 799-803
            std::vector<uint8_t> crc = std::vector<uint8_t>(5, 0);               // 803-808
            std::vector<uint8_t> zeros_res = std::vector<uint8_t>(12, 0);        // 808-820

            void dataToBmsState(const std::vector<uint8_t> &data);
            void dataToWirelessRemoteState(const std::vector<uint8_t> &data);
            void dataToImu(const std::vector<uint8_t> &data);
            void dataToMotorState(const std::vector<uint8_t> &data, int index);

            void parseData(const std::vector<uint8_t> &data);
        };
} // namespace FDSC

#endif
