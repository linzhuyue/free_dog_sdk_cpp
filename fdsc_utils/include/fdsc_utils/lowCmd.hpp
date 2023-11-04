#ifndef _FDSC_LOWCMD_H_
#define _FDSC_LOWCMD_H_
#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
#include <algorithm>
#include <fdsc_utils/enums.hpp>
#include <fdsc_utils/complex.hpp>
#include <fdsc_utils/common.hpp>

namespace FDSC{
    class lowCmd {
        public:
            std::vector<uint8_t> head = {0xFE, 0xEF}; //2bytes
            uint8_t levelFlag = 0xFF;
            uint8_t frameReserve = 0;
            std::vector<uint8_t> SN = {0, 0, 0, 0, 0, 0, 0, 0}; //8bytes
            std::vector<uint8_t> version = {0, 0, 0, 0, 0, 0, 0, 0}; //8bytes
            std::vector<uint8_t> bandWidth = {0x3A, 0xC0}; //2bytes
            MotorCmdArray motorCmd;
            BmsCmd bms = BmsCmd(0, {0, 0, 0});
            std::vector<uint8_t> wirelessRemote = std::vector<uint8_t>(40, 0);
            std::vector<uint8_t> reserve = std::vector<uint8_t>(4, 0);
            std::vector<uint8_t> crc;
            bool encrypt = true;

        inline  std::vector<uint8_t> buildCmd(bool debug = false) {
                std::vector<uint8_t> cmd(614);
                std::copy(head.begin(), head.end(), cmd.begin());
                cmd[2] = levelFlag;
                cmd[3] = frameReserve;
                std::copy(SN.begin(), SN.end(), cmd.begin() + 4);
                std::copy(version.begin(), version.end(), cmd.begin() + 12);
                std::copy(bandWidth.begin(), bandWidth.end(), cmd.begin() + 20);
                std::vector<uint8_t> motorCmdBytes = motorCmd.getBytes();
                std::copy(motorCmdBytes.begin(), motorCmdBytes.end(), cmd.begin() + 22); //22 -562 motorcmd len:27
                std::vector<uint8_t> bmsBytes = bms.getBytes();
                std::copy(bmsBytes.begin(), bmsBytes.end(), cmd.begin() + 562);
                std::copy(wirelessRemote.begin(), wirelessRemote.end(), cmd.begin() + 566);
                std::vector<uint8_t> crcData(cmd.begin(), cmd.end() - 6);
                if (encrypt) {
                    crc = encryptCrc(genCrc(crcData));
                } else {
                    crc = genCrc(crcData);
                }
                std::copy(crc.begin(), crc.end(), cmd.end() - 4);

                if (debug) {
                    std::cout << "Length: " << cmd.size() << std::endl;
                    std::cout << "Data: ";
                    int count_print = 0;
                    for (const auto &byte : cmd) {
                        std::cout <<" 0x"<< std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
                        count_print++;
                        if (count_print % 16==0)
                        {
                            std::cout<<std::endl;
                        }
                        
                    }
                    std::cout << std::dec << std::endl;
                }

                return cmd;
            }
        };

}
#endif