#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <cstdint>
#include <cstdint>
#include <cstring>
#include <bitset>
#include <algorithm>
#include <fdsc_utils/enums.hpp>
#include <fdsc_utils/common.hpp>
#include <fdsc_utils/color.hpp>
namespace FDSC{
    class Cartesian {
        public:
            float x;
            float y;
            float z;

            Cartesian(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    };

    class BmsState {
        public:
            uint8_t version_h;
            uint8_t version_l;
            uint8_t bms_status;
            uint8_t SOC;
            int32_t current;
            uint16_t cycle;
            std::vector<uint8_t> BQ_NTC ;  //2Bytes
            std::vector<uint8_t> MCU_NTC;  //2Bytes
            // TODO here have problem with BMS cell vol
            std::vector<uint16_t> cell_vol; //10 int 

            BmsState(uint8_t vh, uint8_t vl, uint8_t bs, uint8_t soc, int32_t curr, uint16_t cyc, std::vector<uint8_t> &bq, std::vector<uint8_t> mcu, std::vector<uint16_t> &cv)
                : version_h(vh), version_l(vl), bms_status(bs), SOC(soc), current(curr), cycle(cyc), BQ_NTC(bq), MCU_NTC(mcu), cell_vol(cv) {}
        
    };

    class BmsCmd {
        public:
            uint8_t off;
            std::vector<uint8_t> reserve;

            BmsCmd(uint8_t o = 0, const std::vector<uint8_t>& r = {0, 0, 0}) : off(o), reserve(r) {}

            std::vector<uint8_t> getBytes() {
                std::vector<uint8_t> data;
                data.push_back(off);
                data.insert(data.end(), reserve.begin(), reserve.end());
                return data;
            }

            BmsCmd fromBytes(const std::vector<uint8_t>& data) {
                off = data[0];
                for (int i = 0; i < 3; i++) {
                    reserve[i] = data[i + 1];
                }
                return *this;
            }
    };

    class Led {
        public:
            uint8_t r;
            uint8_t g;
            uint8_t b;

            Led(uint8_t _r, uint8_t _g, uint8_t _b) : r(_r), g(_g), b(_b) {}

            std::vector<uint8_t> getBytes() {
                std::vector<uint8_t> data = {r, g, b, 0};
                return data;
            }
    };

    class MotorState {
        public:
            int mode;
            float q;
            float dq;
            float ddq;
            float tauEst;
            float q_raw;
            float dq_raw;
            float ddq_raw;
            float temperature;
            std::vector<uint8_t> reserve; //8Bytes

            MotorState(int m, float q_, float dq_, float ddq_, float tau, float qr, float dqr, float ddqr, float temp, const std::vector<uint8_t>& res)
                : mode(m), q(q_), dq(dq_), ddq(ddq_), tauEst(tau), q_raw(qr), dq_raw(dqr), ddq_raw(ddqr), temperature(temp), reserve(res) {}
            void set_data(int m, float q_, float dq_, float ddq_, float tau, float qr, float dqr, float ddqr, float temp, const std::vector<uint8_t>& res)
        {
            mode = m;
            q = q_;
            dq = dq_;
            ddq = ddq_;
            tauEst = tau;
            q_raw = qr;
            dq_raw = dqr;
            ddq_raw = ddqr;
            temperature = temp;
            reserve = res; //8Bytes
        };
    };

    class Imu {
        public:
            std::vector<float> quaternion; //4 flotas
            std::vector<float> gyroscope; 
            std::vector<float> accelerometer;
            std::vector<float> rpy;
            uint8_t temperature;

            Imu(const std::vector<float>& quat, const std::vector<float>& gyro, const std::vector<float>& accel, const std::vector<float>& r, uint8_t temp)
                : quaternion(quat), gyroscope(gyro), accelerometer(accel), rpy(r), temperature(temp) {}
    };

    class MotorCmd {
        public:
            MotorModeLow mode; //0
            float q ;  //1-5
            float dq; //5-9
            float tau ; //9-11
            float Kp ; //11-13
            float Kd ; //13-15
            std::array<float, 3> reserve; //15:19,19:23,23:27
            //total len is 27 Bytes
            MotorCmd(){};
            MotorCmd(MotorModeLow m, float q_, float dq_, float tau_, float kp, float kd, const std::array<float, 3>& res)
                : mode(m), q(q_), dq(dq_), tau(tau_), Kp(kp), Kd(kd), reserve(res) {}
            void set_data(const MotorModeLow m,const float q_,const float dq_,const float tau_,const float kp_,const float kd_)
            {
                mode = m;
                q = q_;
                dq = dq_;
                tau = tau_;
                Kp = kp_;
                Kd = kd_;
                reserve = {0.0f,0.0f,0.0f}; //8Bytes
            };
            std::vector<uint8_t> getBytes() {
                if (static_cast<int>(mode) < 256) {
                    mode = static_cast<MotorModeLow>(static_cast<int>(mode));
                }
                std::vector<uint8_t> data;
                data.push_back(static_cast<uint8_t>(mode));
                std::vector<uint8_t> qData, dqData, tauData, kpData, kdData;
                qData   = float_to_hex(q);
                dqData  = float_to_hex(dq);
                tauData = tau_to_hex(tau);
                kpData  = kpKd_to_hex(Kp);
                kdData  = kpKd_to_hex(Kd);
                data.insert(data.end(), qData.begin(), qData.end());
                data.insert(data.end(), dqData.begin(), dqData.end());
                data.insert(data.end(), tauData.begin(), tauData.end());
                data.insert(data.end(), kpData.begin(), kpData.end());
                data.insert(data.end(), kdData.begin(), kdData.end());
                for (int i = 0; i < 3; i++)
                {
                    std::vector<uint8_t> res = float_to_hex(reserve[0]);
                    data.insert(data.end(), res.begin(), res.end()); 
                }
                // fromBytes(data,true);
                return data;
            }

            void fromBytes(const std::vector<uint8_t>& data,bool show_data) {
                mode = static_cast<MotorModeLow>(data[0]);
                q = hex_to_float_i(data,1,5);
                dq = hex_to_float_i(data,5,9);
                std::vector<uint8_t> taudata_{data[9],data[10]};
                tau = hex_to_tau(taudata_);
                std::vector<uint8_t> kpdata_{data[11],data[12]};
                Kp = hex_to_kp_kd(kpdata_);
                std::vector<uint8_t> kddata_{data[13],data[14]};
                Kd = hex_to_kp_kd(kddata_);
                for (int i = 0; i < 3; i++)
                {
                    reserve[i] = hex_to_float_i(data,15 + i*4,19 + i*4);
                }
                if (show_data)
                {
                    std::cout<<" mode "<<static_cast<int>(mode)<<std::endl;
                    std::cout<<" q "<<q<<std::endl;
                    std::cout<<" dq "<<dq<<std::endl;
                    std::cout<<" tau "<<tau<<std::endl;
                    std::cout<<" Kp "<<Kp<<std::endl;
                    std::cout<<" Kd "<<Kd<<std::endl;
                    std::cout<<"reserve: "<<reserve[0]<<" "<<reserve[1]<<" "<<reserve[2]<<" "<<std::endl;
                }
                
            }
    };

    class MotorCmdArray {
        public:
            MotorCmd motors[20];

            MotorCmdArray() {
                for (int i = 0; i < 20; ++i) {
                    motors[i] = MotorCmd(MotorModeLow::Servo, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, {0.0f, 0.0f, 0.0f});
                }
            }
            // MotorModeLow m, q_,dq_,tau_,kp_, kd_
            void setMotorCmd(const std::string& motorIndex,const MotorModeLow m_,const std::vector<float> &jointdata) {
                
                int index;
                if (motorIndex == "FR_0" || motorIndex == "0") index = 0;
                else if (motorIndex == "FR_1" || motorIndex == "1") index = 1;
                else if (motorIndex == "FR_2" || motorIndex == "2") index = 2;
                else if (motorIndex == "FL_0" || motorIndex == "3") index = 3;
                else if (motorIndex == "FL_1" || motorIndex == "4") index = 4;
                else if (motorIndex == "FL_2" || motorIndex == "5") index = 5;
                else if (motorIndex == "RR_0" || motorIndex == "6") index = 6;
                else if (motorIndex == "RR_1" || motorIndex == "7") index = 7;
                else if (motorIndex == "RR_2" || motorIndex == "8") index = 8;
                else if (motorIndex == "RL_0" || motorIndex == "9") index = 9;
                else if (motorIndex == "RL_1" || motorIndex == "10") index = 10;
                else if (motorIndex == "RL_2" || motorIndex == "11") index = 11;
                else if (motorIndex == "Unknown1" || motorIndex == "12") index = 12;
                else if (motorIndex == "Unknown2" || motorIndex == "13") index = 13;
                else if (motorIndex == "Unknown3" || motorIndex == "14") index = 14;
                else if (motorIndex == "Unknown4" || motorIndex == "15") index = 15;
                else if (motorIndex == "Unknown5" || motorIndex == "16") index = 16;
                else if (motorIndex == "Unknown6" || motorIndex == "17") index = 17;
                else if (motorIndex == "Unknown7" || motorIndex == "18") index = 18;
                else if (motorIndex == "Unknown8" || motorIndex == "19") index = 19;
                else index = -1; // Unknown motor
                if (jointdata.size() != 5) {
                    throw std::invalid_argument("Input vector must contain 5 floats");
                }
                if (index >= 0) {
                    motors[index].set_data(m_,jointdata[0],jointdata[1],jointdata[2],jointdata[3],jointdata[4]);
                }
            }

            std::vector<uint8_t> getBytes() {
                std::vector<uint8_t> data;
                for (int i = 0; i < 20; ++i) {
                    std::vector<u_int8_t> temp = motors[i].getBytes();
                    data.insert(data.end(), temp.begin(), temp.end());
                }

                return data;
            }
            std::vector<uint8_t> getChunk(const std::vector<uint8_t>& data, int i) {
                std::vector<uint8_t> chunk(data.begin() + (i - 1) * 27, data.begin() + i * 27);
                return chunk;
            }

            void fromBytes(const std::vector<uint8_t>& data) {
                for (int i = 0; i < 20; i++)
                {
                    motors[i].fromBytes(getChunk(data,i),false);
                }
            }
    };
}