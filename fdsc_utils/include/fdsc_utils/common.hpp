#ifndef FDSC_COMMON_H
#define FDSC_COMMON_H
#include <cstring>
#include <iostream>
#include <array>
#include <cstdint>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <bitset>
#include <map>
#include <algorithm>
#include <cassert>
#include <fdsc_utils/enums.hpp>

namespace FDSC{

    constexpr double go1_Hip_max   = 1.047;    // unit:radian ( = 60   degree)
    constexpr double go1_Hip_min   = -1.047;   // unit:radian ( = -60  degree)
    constexpr double go1_Thigh_max = 2.966;    // unit:radian ( = 170  degree)
    constexpr double go1_Thigh_min = -0.663;   // unit:radian ( = -38  degree)
    constexpr double go1_Calf_max  = -0.837;   // unit:radian ( = -48  degree)
    constexpr double go1_Calf_min  = -2.721;   // unit:radian ( = -156 degree)
    // definition of each leg and joint

    std::string lib_version();
    std::string decode_sn(const std::vector<uint8_t>& data);
    std::string decode_version(const std::vector<uint8_t>& data);
    float getVoltage(const std::vector<float>& cellVoltages);

    std::vector<uint8_t> float_to_hex(float f);

    float hex_to_float(const std::vector<uint8_t>& hex);

    float hex_to_float_i(const std::vector<uint8_t>& hex, const int startIndex, const int endIndex);

    int hex_to_int_i(const std::vector<uint8_t>& data, int startIndex, int endIndex);

    uint16_t hex_to_u16_i(const std::vector<uint8_t>& data, int startIndex, int endIndex);

    std::vector<uint8_t> fraction_to_hex(float fraction, bool neg = false);

    std::vector<uint8_t> tau_to_hex(float tau);

    float hex_to_fraction(uint8_t hex_byte, bool neg = false);

    float hex_to_tau(const std::vector<uint8_t>& hex_bytes);

    std::vector<uint8_t> kpKd_to_hex(float KpKd);

    float hex_to_kp_kd(const std::vector<uint8_t>& byte_arr);

    float hex_to_float_2B(const std::vector<uint8_t>& byte_arr ,int startIndex, int endIndex);
    std::vector<uint8_t> genCrc(const std::vector<uint8_t>& i);

    std::vector<uint8_t> encryptCrc(const std::vector<uint8_t>& crc_val);

    void byte_print(const std::vector<uint8_t>& data, bool showbytes);

    void pretty_show_vector(const std::vector<float>& data);

    void show_byte_data(const std::vector<uint8_t>& data, int datalen);

    float jointLinearInterpolation(float initPos, float targetPos, float rate);

    void show_in_lowcmd();
}
#endif // FDSC_COMMON_H

