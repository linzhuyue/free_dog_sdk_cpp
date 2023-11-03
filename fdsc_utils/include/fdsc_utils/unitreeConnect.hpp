#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <atomic>
#include <fdsc_utils/enums.hpp>
#include <fdsc_utils/complex.hpp>
#include <fdsc_utils/common.hpp>
namespace FDSC
{
    const int listenPort = 8090; //UDP server port (Not used)
    const int sendPortLow = 8007;//UDP Low server port
    const int sendPortHigh = 8082; //UDP HIGH server port
    const std::string localIpWifi = "192.168.12.222"; // your PC wifi,will be auto generate from Go1
    const std::string localIpEth = "192.168.123.11"; // Your PC
    const std::string addrWifi = "192.168.12.1"; // wifi pi
    // wired
    const std::string addrLow = "192.168.123.10"; // Nano can't connect
    const std::string addrHigh = "192.168.123.161"; // Pi
    // sim
    const std::string sim_addr = "127.0.0.1"; // Local 
    
    class UnitreeConnection {
    public:
        //default use the wifi high level control 
        UnitreeConnection(const std::string& settings = "HIGH_WIFI_DEFAULTS")
            : listenPort_(listenPort), sendPort_(sendPortHigh), localIp_(localIpWifi) {
            if (settings == "LOW_WIRED_DEFAULTS") { //low level ethernet control
                sendPort_ = sendPortLow;
                localIp_ = localIpEth;
                addr_ = addrLow;
            }else if (settings == "LOW_WIFI_DEFAULTS") //low level wifi control
            {
                sendPort_ = sendPortLow;
                localIp_ = localIpWifi;
                addr_ = addrLow;
            }else if (settings == "HIGH_WIRED_DEFAULTS") //high
            {
                sendPort_ = sendPortHigh;
                localIp_ = localIpEth;
                addr_ = addrHigh;
            }else if(settings == "SIM_DEFAULTS")
            {
                sendPort_ = sendPortHigh;
                localIp_ = sim_addr;
                addr_ = sim_addr;
            }else
            {
                sendPort_ = sendPortHigh;
                localIp_ = localIpWifi;
                addr_ = addrHigh;
            }
            initialize();
        }

        ~UnitreeConnection() {
            stopRecv();
            closeSocket();
        }

        void startRecv() {
            recvThreadID_ = std::thread(&UnitreeConnection::recvThread, this);
        }

        void stopRecv() {
            runRecv_.store(true);
            if (recvThreadID_.joinable()) {
                recvThreadID_.join();
            }
        }

        void send(const std::vector<uint8_t>& cmd) {
            if (socket_ != -1) {
                sockaddr_in serverAddr;
                serverAddr.sin_family = AF_INET;
                serverAddr.sin_port = htons(sendPort_);
                // serverAddr.sin_addr.s_addr = inet_addr(addr_.c_str());
                inet_pton(AF_INET, addr_.c_str(), &(serverAddr.sin_addr));

                if (sendto(socket_, cmd.data(), cmd.size(), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
                    std::cerr << "Error sending data." << std::endl;
                }
            }
        }

        std::vector<std::vector<uint8_t>> getData() {
            std::vector<std::vector<uint8_t>> ret = data_all;
            data_all.clear();
            return ret;
        }

    private:
        int listenPort_;
        int sendPort_;
        std::string addr_;
        std::string localIp_;
        int socket_;
        std::thread recvThreadID_;
        std::atomic<bool> runRecv_;
        // std::vector<uint8_t> data_;
        std::vector<std::vector<uint8_t>> data_all;

        void initialize() {
            runRecv_.store(false);
            socket_ = socket(AF_INET, SOCK_DGRAM, 0); //UDP data stream
            if (socket_ == -1) {
                std::cerr << "Error creating socket." << std::endl;
            }

        }

        void closeSocket() {
            if (socket_ != -1) {
                close(socket_);
            }
        }

        void recvThread() {
            while (!runRecv_.load()) {
                struct sockaddr_in src;
                socklen_t src_len = sizeof(src);
                uint8_t buffer[1200];
                memset(buffer, 0, 1200);
                std::vector<uint8_t> data_;
                try{
                     ssize_t bytesRead = recvfrom(socket_, buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr*)&src, &src_len);
                    if (bytesRead == -1) {
                        std::cout<<"No data continue!"<<std::endl;
                        continue;
                    }
                    
                    data_.insert(data_.end(), buffer, buffer + bytesRead); //add the bytesRead length new data from vec end,
                    data_all.push_back(data_);
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100)); //wait all the data recv finished
            }

            
        }
    };
} // namespace FDSC