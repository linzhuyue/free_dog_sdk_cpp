#ifndef _FDSC_ENUMS_H_
#define _FDSC_ENUMS_H_
#include <stdint.h>
namespace FDSC{
    enum class ROBOTModeHigh {  
        IDLE = 0,  
        FORCE_STAND = 1,  
        VEL_WALK = 2, // 与POS_WALK相同  
        POS_WALK = 3,  
        PATH = 4, // 不工作  
        STAND_DOWN = 5,  
        STAND_UP = 6,  
        DAMPING = 7,  
        RECOVERY = 8, // 恢复到站立状态  
        BACKFLIP = 9,  
        JUMPYAW = 10,  
        STRAIGHTHAND = 11, // bai year 
        DANCE1 = 12, // 与DANCE2不同  
        DANCE2 = 13 // 与DANCE1不同  
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

}// end FDSC
#endif