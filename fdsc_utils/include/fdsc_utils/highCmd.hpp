#ifndef _FDSC_HIGHCMD_H_
#define _FDSC_HIGHCMD_H_

#include <iostream>
#include <iomanip>
#include <vector>
#include <fdsc_utils/complex.hpp>
#include <fdsc_utils/common.hpp>
namespace FDSC{
    class highCmd {
        public:
            highCmd(){};
            std::vector<uint8_t> head = {0xFE, 0xEF};
            uint8_t levelFlag = 0x00;
            uint8_t frameReserve = 0;
            std::vector<uint8_t> SN = {0, 0, 0, 0, 0, 0, 0, 0};
            std::vector<uint8_t> version = {0, 0, 0, 0, 0, 0, 0, 0};
            std::vector<uint8_t> bandWidth = {0, 0};
            ROBOTModeHigh robotmode_ =  ROBOTModeHigh::IDLE;
            GaitType gaitType = GaitType::IDLE;
            SpeedLevel speedLevel = SpeedLevel::LOW_SPEED;
            float footRaiseHeight = 0.0;
            float bodyHeight = 0.0; // TODO this can only set to negtive when the robot is standup, positive is add to the current height
            std::vector<float> position = {0.0, 0.0};
            std::vector<float> euler = {0.0, 0.0, 0.0};
            std::vector<float> velocity = {0.0, 0.0};
            float yawSpeed = 0.0;
            BmsCmd bms = BmsCmd(0, {0, 0, 0});
            Led led = Led(0, 0, 0);
            std::vector<uint8_t> wirelessRemote = std::vector<uint8_t>(40, 0); //wifi
            std::vector<uint8_t> reserve = std::vector<uint8_t>(4, 0);
            std::vector<uint8_t> crc;
            bool encrypt = false;
            std::vector<uint8_t> buildCmd(bool debug = false);

    };
}// end FDSC
#endif