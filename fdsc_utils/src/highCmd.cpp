#include <fdsc_utils/highCmd.hpp>
namespace FDSC {

    std::vector<uint8_t> highCmd::buildCmd(bool debug) {
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
}