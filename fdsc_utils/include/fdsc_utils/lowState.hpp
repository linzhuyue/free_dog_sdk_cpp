#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <fdsc_utils/enums.hpp>
#include <fdsc_utils/complex.hpp>
#include <fdsc_utils/common.hpp>
namespace FDSC
{
    class lowState {
        public:
                // all data in little endian data len 807
                std::vector<uint8_t> head{0, 0};  //0-2
                uint8_t levelFlag = 0; //2
                uint8_t frameReserve = 0;//3
                std::vector<uint8_t> SN{0, 0, 0, 0, 0, 0, 0, 0}; //4-12
                std::vector<uint8_t> version{0, 0, 0, 0, 0, 0, 0, 0}; //12-20
                std::vector<uint8_t> bandWidth{0, 0};//20-22

                std::vector<float> imu_quaternion = std::vector<float>(4,0.0f); //4 flotas
                std::vector<float> imu_gyroscope = std::vector<float>(3,0.0f); 
                std::vector<float> imu_accelerometer = std::vector<float>(3,0.0f);
                std::vector<float> imu_rpy = std::vector<float>(3,0.0f);
                uint8_t temperature_imu = 0;
                Imu imu_ = Imu(imu_quaternion,imu_gyroscope,imu_accelerometer,imu_rpy,temperature_imu); // 22-75
                //motor state: 20 * 32 in lowstate
                int motormode = 0;
                float q = 0;
                float dq = 0;
                float ddq = 0;
                float tauEst = 0;
                float q_raw = 0;
                float dq_raw = 0;
                float ddq_raw = 0;            
                uint8_t motor_temperature = 0;
                std::vector<uint8_t> res_bms{0,0,0,0,0,0,0,0};
                std::vector<MotorState> motorstate =std::vector<MotorState>(20, MotorState(motormode, q, dq, ddq, tauEst, q_raw, dq_raw, ddq_raw, motor_temperature, res_bms)); 
                uint8_t version_h = 0;
                uint8_t version_l = 0;
                uint8_t bms_status = 0;
                uint8_t SOC = 0;
                int32_t current = 0;
                uint16_t cycle = 0;
                std::vector<uint8_t> BQ_NTC{0,0} ;  //2Bytes
                std::vector<uint8_t> MCU_NTC{0,0};  //2Bytes
                // TODO here have problem with BMS cell vol, you don't care BMS data is zero in GO1 air
                std::vector<uint16_t> cell_vol = std::vector<uint16_t>(10,0); //10 int 
                BmsState bm_s= BmsState(version_h, version_l, bms_status, SOC, current, cycle, BQ_NTC, MCU_NTC, cell_vol); // 715-739
                std::vector<uint16_t> footForce = {0, 0, 0, 0}; //739-747. 2Bytes for every
                std::vector<uint16_t> footForceEst = {0, 0, 0, 0}; //747-755
                std::vector<uint8_t> robotlowmode = std::vector<uint8_t>(4,0);  // TODO what ? 755-759
                std::vector<uint8_t> wirelessRemote = std::vector<uint8_t>(40,0);//759-799
                std::vector<uint8_t> reserve = std::vector<uint8_t>(4,0);//799-803
                std::vector<uint8_t> crc = std::vector<uint8_t>(5,0);//803-808
                std::vector<uint8_t> zeros_res = std::vector<uint8_t>(12,0);//808-820
                void dataToBmsState(const std::vector<uint8_t>& data) {
                        version_h = data[0];
                        version_l = data[1];
                        bms_status = data[2];
                        SOC = data[3];
                        current = hex_to_int_i(data, 4, 8);
                        cycle = hex_to_u16_i(data, 8, 10);
                        BQ_NTC[0] =  data[10];
                        BQ_NTC[1] =  data[11];
                        MCU_NTC[0] = data[12];
                        MCU_NTC[1] = data[13];

                        for (int i = 0; i < 10; i++) {
                            cell_vol[i] = hex_to_u16_i(data, 13 + i * 2, 15 + i * 2);
                        }
                    }

                void dataToImu(const std::vector<uint8_t>& data) {
                        for (int i = 0; i < 4; i++) {
                            imu_quaternion[i] = hex_to_float_i(data, i * 4, (i + 1) * 4);
                        }
                        for (int i = 0; i < 3; i++) {
                            imu_gyroscope[i] = hex_to_float_i(data, 16 + i * 4, 16 + (i + 1) * 4);
                            imu_accelerometer[i] = hex_to_float_i(data, 28 + i * 4, 28 + (i + 1) * 4);
                            imu_rpy[i] = hex_to_float_i(data, 40 + i * 4, 40 + (i + 1) * 4);
                        }
                        temperature_imu = data[52];
                    }
                void dataToMotorState(const std::vector<uint8_t>& data,int index) {
                        motormode =int(data[0]);
                        q = hex_to_float_i(data,   1,   5);
                        dq = hex_to_float_i(data,   5,   9);
                        ddq = static_cast<float>(hex_to_u16_i(data,   9,   11));
                        tauEst = static_cast<float>(hex_to_u16_i(data,   11,   13)) * 0.00390625; //TODO why? do not understand?
                        q_raw = hex_to_float_i(data,   13,   17);
                        dq_raw = hex_to_float_i(data,   17,   21);
                        ddq_raw = static_cast<float>(hex_to_u16_i(data,   21,   23));
                        motor_temperature = data[24];
                        std::copy(data.begin()+24,data.end(),res_bms.begin());
                        motorstate[index].set_data(motormode, q, dq, ddq, tauEst, q_raw, dq_raw, ddq_raw, motor_temperature, res_bms);
                    }


                void parseData(const std::vector<uint8_t>& data) {
                    try
                    {
                         // data length is 820
                        std::copy(data.begin(),data.begin()+2, head.begin()); //0-2
                        levelFlag = data[2];
                        frameReserve = data[3];
                        std::copy(data.begin()+4,data.begin()+12, SN.begin()); //4-12
                        std::copy(data.begin()+12,data.begin()+20, version.begin()); //12-20
                        bandWidth[0] = data[20];
                        bandWidth[1] = data[21];
                        std::vector<uint8_t> imu_data(data.begin()+22,data.begin()+75);//22-75
                        dataToImu(imu_data);

                        for (int i = 0; i < 20; i++) {
                            std::vector<uint8_t> motor_tmp_data(data.begin()+75 + i*32,data.begin()+32+75 + i*32);
                            dataToMotorState(motor_tmp_data,i);
                        }
                        
                        std::vector<uint8_t> bms_data(data.begin()+715,data.begin()+739);
                        dataToBmsState(bms_data);
                        // we do not have this in go 1 air
                        for (int i = 0; i < 4; i++) {
                            footForce[i] = hex_to_u16_i(data, 739 + i * 2, 741 + i * 2);
                            footForceEst[i] = hex_to_u16_i(data, 747 + i * 2, 749 + i * 2);
                        }
                        std::copy(data.begin()+755,data.begin()+759, robotlowmode.begin()); //
                        std::copy(data.begin()+759,data.begin()+799, wirelessRemote.begin()); //
                        std::copy(data.begin()+799,data.begin()+803, reserve.begin()); //
                        std::copy(data.begin()+803,data.begin()+808, crc.begin()); //
                        std::copy(data.begin()+808,data.end(), zeros_res.begin()); //808-820
                    }
                    catch(const std::exception& e)
                    {
                        
                        int count_print = 0;
                        for (const auto &byte : data) {
                            std::cout <<" 0x"<< std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
                            count_print++;
                            if (count_print % 16==0)
                            {
                                std::cout<<std::endl;
                            }
                            
                        }
                        std::cout << std::dec << std::endl;
                        std::cerr << e.what() << '\n';
                    }
                    

                }
    };

} // namespace FDSC