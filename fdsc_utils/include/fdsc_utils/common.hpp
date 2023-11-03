#pragma once
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
#include <fdsc_utils/color.hpp>
namespace FDSC{
    enum class DogType {
        UNKNOWN,
        Laikago,
        Aliengo,
        A1,
        Go1,
        B1
    };

    enum class ModelType {
        UNKNOWN,
        AIR,
        PRO,
        EDU,
        PC,
        XX
    };
        std::map<std::string, int> jointMapping = {
        {"FR_0", 0}, {"FR_1", 1}, {"FR_2", 2},
        {"FL_0", 3}, {"FL_1", 4}, {"FL_2", 5},
        {"RR_0", 6}, {"RR_1", 7}, {"RR_2", 8},
        {"RL_0", 9}, {"RL_1", 10}, {"RL_2", 11}
    };

    std::string lib_version() {
        return "0.2";
    }

    std::string decode_sn(const std::vector<uint8_t>& data) {
        DogType type = static_cast<DogType>(data[0]);
        std::string type_name = "UNKNOWN";

        switch (type) {
            case DogType::Laikago: type_name = "Laikago"; break;
            case DogType::Aliengo: type_name = "Aliengo"; break;
            case DogType::A1: type_name = "A1"; break;
            case DogType::Go1: type_name = "Go1"; break;
            case DogType::B1: type_name = "B1"; break;
        }

        ModelType model = static_cast<ModelType>(data[1]);
        std::string model_name = "UNKNOWN";

        switch (model) {
            case ModelType::AIR: model_name = "AIR"; break;
            case ModelType::PRO: model_name = "PRO"; break;
            case ModelType::EDU: model_name = "EDU"; break;
            case ModelType::PC: model_name = "PC"; break;
            case ModelType::XX: model_name = "XX"; break;
        }

        std::string product_name = type_name + "_" + model_name;
        std::string id = std::to_string(data[2]) + "-" + std::to_string(data[3]) + "-" + std::to_string(data[4]) + "[" + std::to_string(data[5]) + "]";

        return product_name + " " + id;
    }

    std::string decode_version(const std::vector<uint8_t>& data) {
        std::string hardware_version = " HardV: " + std::to_string(data[0]) + "." + std::to_string(data[1]) + "." + std::to_string(data[2]);
        std::string software_version = " SoftV: " + std::to_string(data[3]) + "." + std::to_string(data[4]) + "." + std::to_string(data[5]);
        return hardware_version +"\t" + software_version;
    }

    float getVoltage(const std::vector<float>& cellVoltages) {
        float sum = 0.0;
        for (float voltage : cellVoltages) {
            sum += voltage;
        }
        return sum;
    }

    std::vector<uint8_t> float_to_hex(float f) {
        std::vector<uint8_t> result(4);
        uint32_t raw;
        std::memcpy(&raw, &f, sizeof(float));
        for (int i = 0; i < 4; ++i) {
            result[i] = (raw >> (8 * i)) & 0xFF;
        }
        return result;
    }

    float hex_to_float(const std::vector<uint8_t>& hex) {
        uint32_t raw = 0;
        for (int i = 0; i < 4; ++i) {
            raw |= static_cast<uint32_t>(hex[i]) << (8 * i);
        }
        float f;
        std::memcpy(&f, &raw, sizeof(float));
        return f;
    }
    float hex_to_float_i(const std::vector<uint8_t>& hex,const int startIndex, const int endIndex) {
        uint32_t raw = 0;
        assert(endIndex-startIndex!=4);
        for (int i = 0; i < 4 ; ++i) {
            raw |= static_cast<uint32_t>(hex[i + startIndex]) << (8 * i);
        }
        float f;
        std::memcpy(&f, &raw, sizeof(float));
        return f;
    }
    int hex_to_int_i(const std::vector<uint8_t>& data, int startIndex, int endIndex) {
        // Little Endian
        assert(endIndex-startIndex!=4);
        int result = 0;
        for (int i = startIndex; i < endIndex; i++) {
            result = (result << 8) | data[i];
        }
        return result;
    }

    uint16_t hex_to_u16_i(const std::vector<uint8_t>& data, int startIndex, int endIndex) {
            assert(endIndex-startIndex!=2);
            uint16_t result = 0;
            for (int i = startIndex; i < endIndex; i++) {
                result = (result << 8) | data[i];
            }
            return result;
        }
    std::vector<uint8_t> fraction_to_hex(float fraction, bool neg = false) {
        if (fraction == 0.0f) {
            neg = false;
        }
        int hex_value = static_cast<int>(fraction * 256);
        if (neg) {
            hex_value = 255 + hex_value + 1;
        }
        return {static_cast<uint8_t>(hex_value)};
    }

    std::vector<uint8_t> tau_to_hex(float tau) {

        tau = std::roundf(tau * 100) / 100; // Round to 2 decimal places
        int integer_part = static_cast<int>(tau);
        float fractional_part = tau - integer_part;
        bool neg = false;
        if (tau < 0) {
            neg = true;
            integer_part = 255 + integer_part;
        }

        std::vector<uint8_t> result;
        result.push_back(static_cast<uint8_t>(integer_part));
        result.push_back(fraction_to_hex(fractional_part, neg)[0]);
        std::reverse(result.begin(),result.end());
        return result;
    }
    float hex_to_fraction(uint8_t hex_byte, bool neg = false) {
        if (neg) {
            return -1.0f + std::round(static_cast<float>(hex_byte) / 256.0f * 100.0f) / 100.0f;
        } else {
            return std::round(static_cast<float>(hex_byte) / 256.0f * 100.0f) / 100.0f;
        }
    }

    float hex_to_tau(const std::vector<uint8_t>& hex_bytes) {
        uint8_t int_part = hex_bytes[1];
        uint8_t frac_part = hex_bytes[0];

        int int_val = static_cast<int>(int_part);
        bool neg = false;

        // We assume 126 as the tip-over point for negative values
        if (int_val > 126) {
            neg = true;
            int_val = -255 + int_val;
        }

        float fraction = hex_to_fraction(frac_part, neg);

        return static_cast<float>(int_val) + fraction;
    }
    std::vector<uint8_t> kpKd_to_hex(float KpKd) {
        if (KpKd < 0.0 || KpKd > 550.0) {
            throw std::invalid_argument("Input KpKd must positive. Or you should small than 550.0");
        }
        // we can not set KpKd negtive
        int base = static_cast<int>(KpKd);
        float frac = std::round((KpKd - base) * 10.0f);
        int val = 0;
        if (frac < 5) {
            val = (base * 32) + static_cast<int>(frac * 3);
        } else {
            val = (base * 32) + (static_cast<int>(frac - 1) * 3) + 4;
        }
        std::vector<uint8_t> kp_bytes(2);
        kp_bytes[0] = static_cast<uint8_t>(val & 0xFF);
        kp_bytes[1] = static_cast<uint8_t>((val >> 8) & 0xFF);
        return kp_bytes;
    }

    float hex_to_kp_kd(const std::vector<uint8_t>& byte_arr) {
        // Only one decimal place accuracy is guaranteed.
        if (byte_arr.size() != 2) {
            throw std::invalid_argument("Input byte_arr must contain exactly two bytes.");
        }
        // Interpret the two bytes as a little-endian uint16_t
        uint16_t val = static_cast<uint16_t>((byte_arr[1] << 8) | byte_arr[0]);
        int base = val / 32;
        int remainder = val % 32;
        float frac = 0.0f;
        if (remainder < 15) {
            frac = static_cast<float>(remainder) / 3.0f;
        } else {
            frac = (static_cast<float>(remainder - 4) / 3.0f) + 1.0f;
        }
        float result = static_cast<float>(base) + frac * 0.1f;

        return result;
    }
    std::vector<uint8_t> genCrc(const std::vector<uint8_t>& i) {
        uint32_t crc = 0xFFFFFFFF;
        size_t iSize = i.size() / 4;

        if (iSize * 4 != i.size()) {
            throw std::invalid_argument("Input size must be a multiple of 4 bytes");
        }

        for (size_t j = 0; j < iSize; ++j) {
            uint32_t jValue;
            std::memcpy(&jValue, i.data() + (j * 4), sizeof(uint32_t));
            // std::cout<<"jValue"<<jValue<<std::endl;

            for (int b = 0; b < 32; ++b) {
                uint32_t x = (crc >> 31) & 1;
                crc <<= 1;
                crc &= 0xFFFFFFFF;
                if (x ^ (1 & (jValue >> (31 - b)))) {
                    crc ^= 0x04c11db7;
                }
            }
        }

        std::vector<uint8_t> result(4);
        std::memcpy(result.data(), &crc, sizeof(uint32_t));
        return result;
    }

    std::vector<uint8_t> encryptCrc(const std::vector<uint8_t>& crc_val) {
        if (crc_val.size() != 4) {
            // Handle the case where the input vector doesn't have exactly 4 bytes
            throw std::invalid_argument("Input vector must contain 4 bytes");
        }

        uint32_t crc = 0;
        for (size_t i = 0; i < 4; ++i) {
            crc |= (crc_val[i] << (8 * i));
        }

        // Not my idea to call this an encryption, the manufacturer calls it an encryption in the original lib ;)
        uint32_t xor_val = 0xedcab9de;
        uint32_t val = crc ^ xor_val;

        // Swap bytes in little-endian format
        std::vector<uint8_t> data(4);
        data[2] = static_cast<uint8_t>((val >> 24) & 0xFF);
        data[1] = static_cast<uint8_t>((val >> 16) & 0xFF);
        data[0] = static_cast<uint8_t>((val >> 8) & 0xFF);
        data[3] = static_cast<uint8_t>(val & 0xFF);

        return data;
    }
    void byte_print(const std::vector<uint8_t>& data,bool showbytes) {
        if (showbytes)
        {
            for (uint8_t byte : data) {
                std::cout << static_cast<unsigned>(byte) << " ";
            }
        }
    }
    void pretty_show_vector(const std::vector<float>& data)
    {
        for (float byte : data) {
            std::cout << byte << " ";
        }
    }
    void show_byte_data(const std::vector<uint8_t>& data, int datalen)
    {
        int count = 0;
        for (auto packet : data) {
            std::cout <<"0x"<<std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(packet)<<", ";
            if (count % datalen==0 && count!=0)
            {
                std::cout<<std::endl;
            }
            
            count++;
        }
    }
    float jointLinearInterpolation(float initPos, float targetPos, float rate) {
        rate = std::min(std::max(rate, 0.0f), 1.0f);
        return initPos * (1 - rate) + targetPos * rate;
    }
    void show_in_lowcmd()
    {
        std::cout<<SetForeGRN<<"Before Run in lowlevel mode! You should do the following steps to your dog!!"<<std::endl;
        // std::cout <<"Step1: "<< "L2 + A" << std::endl;
        // std::cout <<"Step2: "<< "L2 + A" << std::endl;
        std::cout <<"Step3: "<< "L2 + B" << std::endl;
        std::cout <<"Step4: "<< "L1 + L2 + START" <<RESETTEXT<<std::endl;
    }
} 
// end FDSC NAMESPACE

