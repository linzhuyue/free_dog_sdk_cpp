#ifndef _FDSC_HIGHCMD_H_
#define _FDSC_HIGHCMD_H_

#include <iostream>
#include <iomanip>
#include <vector>
#include <fdsc_utils/enums.hpp>
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

        inline  std::vector<uint8_t> buildCmd(bool debug = false) {
                std::vector<uint8_t> cmd(129);
                std::copy(head.begin(), head.end(), cmd.begin());
                cmd[2] = levelFlag;
                cmd[3] = frameReserve;
                std::copy(SN.begin(), SN.end(), cmd.begin()+4);
                std::copy(version.begin(), version.end(), cmd.begin() + 12);
                std::copy(bandWidth.begin(), bandWidth.end(), cmd.begin() + 20);
                if ( robotmode_ ==  ROBOTModeHigh::IDLE) {
                    cmd[22] = 0;
                } else {
                    cmd[22] = static_cast<uint8_t>( robotmode_);
                }
                if (gaitType == GaitType::IDLE) {
                    cmd[23] = 0;
                } else {
                    cmd[23] = static_cast<uint8_t>(gaitType);
                }
                if (speedLevel == SpeedLevel::LOW_SPEED) {
                    cmd[24] = 0;
                } else {
                    cmd[24] = static_cast<uint8_t>(speedLevel);
                }

                auto floatToHex = [&](float f, int offset) {
                    std::vector<uint8_t> hexBytes = float_to_hex(f);
                    std::copy(hexBytes.begin(), hexBytes.end(), cmd.begin() + offset);
                };

                floatToHex(footRaiseHeight, 25);
                floatToHex(bodyHeight, 29);
                floatToHex(position[0], 33);
                floatToHex(position[1], 37);
                floatToHex(euler[0], 41);
                floatToHex(euler[1], 45);
                floatToHex(euler[2], 49);
                floatToHex(velocity[0], 53);
                floatToHex(velocity[1], 57);
                floatToHex(yawSpeed, 61);
                std::vector<uint8_t> bmsBytes = bms.getBytes();
                std::copy(bmsBytes.begin(), bmsBytes.end(), cmd.begin() + 65);
                std::vector<uint8_t> ledBytes = led.getBytes();
                std::copy(ledBytes.begin(), ledBytes.end(), cmd.begin() + 69);
                std::copy(wirelessRemote.begin(), wirelessRemote.end(), cmd.begin() + 73);
                std::copy(reserve.begin(), reserve.end(), cmd.begin() + 113);
                std::vector<u_int8_t> tmp_cmd(cmd.begin(),cmd.begin() + 124); 
                std::vector<uint8_t> crcData(cmd.begin(), cmd.end() - 5);
                if (encrypt) {
                    crc = encryptCrc(genCrc(crcData));
                } else {
                    crc = genCrc(crcData);
                }
                std::copy(crc.begin(), crc.end(), cmd.end() - 4);
                if (debug) {
                    std::cout << "Send Data (" << cmd.size() << "): ";
                    for (const auto& byte : cmd) {
                         std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte)<< " ";
                    }
                    std::cout << "\n";
                }
                return cmd;
            }
    };
}// end FDSC
#endif