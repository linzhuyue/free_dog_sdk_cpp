#ifndef _FDSC_LOWCMD_H_
#define _FDSC_LOWCMD_H_

#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <fdsc_utils/complex.hpp>
#include <fdsc_utils/common.hpp>

namespace FDSC {
    class lowCmd {
    public:
        std::vector<uint8_t> head = {0xFE, 0xEF}; // 2 bytes
        uint8_t levelFlag = 0xFF;
        uint8_t frameReserve = 0;
        std::vector<uint8_t> SN = {0, 0, 0, 0, 0, 0, 0, 0}; // 8 bytes
        std::vector<uint8_t> version = {0, 0, 0, 0, 0, 0, 0, 0}; // 8 bytes
        std::vector<uint8_t> bandWidth = {0x3A, 0xC0}; // 2 bytes
        MotorCmdArray motorCmd;
        BmsCmd bms = BmsCmd(0, {0, 0, 0});
        std::vector<uint8_t> wirelessRemote = std::vector<uint8_t>(40, 0);
        std::vector<uint8_t> reserve = std::vector<uint8_t>(4, 0);
        std::vector<uint8_t> crc;
        bool encrypt = true;

        std::vector<uint8_t> buildCmd(bool debug = false);
    };
}

#endif
