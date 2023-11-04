#ifndef _FDSC_ENUMS_H_
#define _FDSC_ENUMS_H_

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
        TROT_OBSTACLE = 4 //我们的go1没有这个  
    };  
    
    enum class SpeedLevel {  
        LOW_SPEED = 0,  
        MEDIUM_SPEED = 1,  
        HIGH_SPEED = 2  
    };  
    
    enum class Motor {  
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
    
}// end FDSC
#endif