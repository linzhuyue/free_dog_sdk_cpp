/*
 * @Author: linzhuyue cboy@mail.ustc.edu.cn
 * @Date: 2023-10-20 18:18:36
 * @LastEditors: linzhuyue cboy@mail.ustc.edu.cn
 * @LastEditTime: 2023-10-30 15:00:45
 * @FilePath: /code_should_rewrite/ros_ws/src/free_dog_sdk_cpp/fdsc_utils/src/test/test.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cmath>
#include <fdsc_utils/common.hpp>
#include <iomanip>
#include <bits/stdc++.h>

int main()
{

    // std::vector<uint8_t> test= FDSC::float_to_hex(0.0);
    // for (auto i:test)
    // {
    //     std::cout<<std::hex<<" "<<i<<" ";
    // }
    // for (auto byte : test) { 
    //     // std::cout<<std::hex<<byte<<std::endl;
    //     std::cout <<" 0x"<< std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    // }
    // std::cout<<std::endl;
    std::vector<uint8_t> test1{0x08, 0xa1, 0x3f, 0x3d};

    // float data = FDSC::hex_to_float(test1);
    // std::cout<<"data "<<data<<std::endl;
    std::vector<uint8_t> crcdata=FDSC::genCrc(test1);
    for (auto byte : crcdata) { 
        // std::cout<<std::hex<<byte<<std::endl;
        std::cout <<" 0x" <<std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    std::cout<<std::endl;
    std::vector<uint8_t> crcdataen = FDSC::encryptCrc(crcdata);
    for (auto byte : crcdataen) { 
        // std::cout<<std::hex<<byte<<std::endl;
        std::cout <<" 0x"<< std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    std::cout<<std::endl;
    // // tau to hex
    // float tau = 0.4;
    // std::vector<uint8_t> test2= FDSC::tau_to_hex(tau);
    // // for (auto i:test)
    // // {
    // //     std::cout<<std::hex<<" "<<i<<" ";
    // // }
    // for (auto byte : test2) { 
    //     // std::cout<<std::hex<<byte<<std::endl;
    //     std::cout <<"-0x" <<std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    // }
    // std::cout<<std::endl;
    // std::cout<<"tau float: "<<tau<<" after: "<<FDSC::hex_to_tau(test2)<<std::endl;
    return 0;
}