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
#include <stdint.h>
namespace FDSC{

    constexpr double go1_Hip_max   = 1.047;    // unit:radian ( = 60   degree)
    constexpr double go1_Hip_min   = -1.047;   // unit:radian ( = -60  degree)
    constexpr double go1_Thigh_max = 2.966;    // unit:radian ( = 170  degree)
    constexpr double go1_Thigh_min = -0.663;   // unit:radian ( = -38  degree)
    constexpr double go1_Calf_max  = -0.837;   // unit:radian ( = -48  degree)
    constexpr double go1_Calf_min  = -2.721;   // unit:radian ( = -156 degree)
    // definition of each leg and joint
    enum class ROBOTModeHigh {  
        IDLE = 0,  
        FORCE_STAND = 1,  
        VEL_WALK = 2, //   
        POS_WALK = 3,  
        PATH = 4, //  
        STAND_DOWN = 5,  
        STAND_UP = 6,  
        DAMPING = 7,  
        RECOVERY = 8, //   
        BACKFLIP = 9,  
        JUMPYAW = 10,  
        STRAIGHTHAND = 11, // year call 
        DANCE1 = 12, // 
        DANCE2 = 13 // 
    };  
    
    enum class GaitType {  
        IDLE = 0,  
        TROT = 1,  
        TROT_RUNNING = 2,  
        CLIMB_STAIR = 3,  
        TROT_OBSTACLE = 4 //
    };  
    
    enum class SpeedLevel {  
        LOW_SPEED = 0,  
        MEDIUM_SPEED = 1,  
        HIGH_SPEED = 2  
    };  
    enum class Leg : int {  
        FR_ = 0,       // leg index
        FL_ = 1,
        RR_ = 2,
        RL_ = 3
    };  
    enum class Motor : int{  
        FR_0 = 0,  
        FR_1 = 1,  
        FR_2 = 2,  
        FL_0 = 3,  
        FL_1 = 4,  
        FL_2 = 5,  
        RR_0 = 6,  
        RR_1 = 7,  
        RR_2 = 8,  
        RL_0 = 9,  
        RL_1 = 10,  
        RL_2 = 11  
    };  
    
    enum class MotorModeLow {  
        Damping = 0x00,  
        Servo = 0x0A,  
        Overheat = 0x08  
    };  

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
    
    // 16b
    typedef union {
        struct {
            uint8_t R1          :1;
            uint8_t L1          :1;
            uint8_t start       :1;
            uint8_t select      :1;
            uint8_t R2          :1;
            uint8_t L2          :1;
            uint8_t F1          :1;
            uint8_t F2          :1;
            uint8_t A           :1;
            uint8_t B           :1;
            uint8_t X           :1;
            uint8_t Y           :1;
            uint8_t up          :1;
            uint8_t right       :1;
            uint8_t down        :1;
            uint8_t left        :1;
        } components;
        uint16_t value;
    } xKeySwitchUnion;

    // 40 Byte (now used 24B)
    typedef struct {
        uint8_t head[2]; //
        xKeySwitchUnion btn; //16B
        float lx; //2B
        float rx; //2B 
        float ry; //2B
        float L2; 
        float ly; //2B

        uint8_t idle[16]; //16B  16B + 8B = 24B the former 24B
    } xRockerBtnDataStruct;

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

