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
#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace FDSC
{
    const int sendPortLow = 8007; // UDP Low server port
    const int sendPortHigh = 8082; // UDP HIGH server port
    const std::string localIpWifi = "192.168.12.222"; // Your PC wifi will give by the DHCP
    const std::string localIpEth = "192.168.123.12"; // Your PC
    const std::string addrWifi = "192.168.12.1"; // wifi pi
    const std::string addrLow = "192.168.123.10"; // Nano can't connect
    const std::string addrHigh = "192.168.123.161"; // Pi wired IP
    const std::string sim_addr = "127.0.0.1"; // Local

    class UnitreeConnection
    {
    public:
        UnitreeConnection(const std::string &settings = "HIGH_WIFI_DEFAULTS")
            : sendPort_(sendPortHigh), localIp_(localIpWifi)
        {
            if (settings == "LOW_WIRED_DEFAULTS")
            { // low level ethernet control
                sendPort_ = sendPortLow;
                localIp_ = localIpEth;
                addr_ = addrLow;
            }
            else if (settings == "LOW_WIFI_DEFAULTS")
            { // low level wifi control
                sendPort_ = sendPortLow;
                localIp_ = localIpWifi;
                addr_ = addrLow;
            }
            else if (settings == "HIGH_WIRED_DEFAULTS")
            { // high level ethernet control
                sendPort_ = sendPortHigh;
                localIp_ = localIpEth;
                addr_ = addrHigh;
            }
            else if (settings == "SIM_DEFAULTS")
            {
                sendPort_ = sendPortHigh;
                localIp_ = sim_addr;
                addr_ = sim_addr;
            }
            else
            {
                sendPort_ = sendPortHigh;
                localIp_ = localIpWifi;
                addr_ = addrHigh;
            }
            initialize();
        }

        ~UnitreeConnection()
        {
            stopRecv();
        }

        void startRecv()
        {
            runRecv_.store(true);
            recvThread_ = std::thread(&UnitreeConnection::recvThread, this);
            recvThread_.detach(); //run in back
        }

        void stopRecv()
        {
            runRecv_.store(false);
            io_context->run();
            if (recvThread_.joinable())
            {
                recvThread_.join();
            }
        }

        void send(const std::vector<uint8_t> &cmd)
        {
            if (socket_ && socket_->is_open())
            {
                std::lock_guard<std::mutex> lock(data_mutex);
                socket_->send_to(boost::asio::buffer(cmd), *receiver_endpoint_);
            }
        }
        std::vector<std::vector<uint8_t>> getData()
        {
            std::lock_guard<std::mutex> lock(data_mutex);
            std::vector<std::vector<uint8_t>> ret = data_all;
            data_all.clear();
            return ret;
        }
        void getData(std::vector<std::vector<uint8_t>>& ret)
        {
            try
            {
                std::lock_guard<std::mutex> lock(data_mutex);
                ret = data_all;
                data_all.clear();
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            

        }

    private:
        int sendPort_;
        
        std::string addr_;
        std::string localIp_;
        std::shared_ptr<boost::asio::io_context> io_context;
        std::shared_ptr<boost::asio::ip::udp::socket> socket_;
        std::shared_ptr<boost::asio::ip::udp::endpoint> receiver_endpoint_;
        std::thread recvThread_;
        std::atomic<bool> runRecv_;
        std::vector<std::vector<uint8_t>> data_all;
        std::mutex data_mutex;

        void initialize()
        {
            io_context = std::make_shared<boost::asio::io_context>();
            socket_ = std::make_shared<boost::asio::ip::udp::socket>(*io_context);
            receiver_endpoint_ = std::make_shared<boost::asio::ip::udp::endpoint>(
                boost::asio::ip::address::from_string(addr_), sendPort_);
            socket_->open(boost::asio::ip::udp::v4());
        }

        void recvThread()
        {
            while (runRecv_.load())
            {
                std::vector<uint8_t> recv_buffer(2048);
                std::vector<uint8_t> data_;
                boost::system::error_code error;
                try{
                    std::size_t bytesRead = socket_->receive_from(
                    boost::asio::buffer(recv_buffer), *receiver_endpoint_,0,error);
                    
                    if (error && error != boost::asio::error::message_size) {
                        std::cout<<"error in udp boost "<<std::endl;
                        throw boost::system::system_error(error);
                    }
                   if (bytesRead > 0)
                    {
                        recv_buffer.resize(bytesRead);
                        data_.insert(data_.end(), recv_buffer.begin(), recv_buffer.end());
                        std::lock_guard<std::mutex> lock(data_mutex);
                        data_all.push_back(data_);
                    }
                }
                catch(const std::exception& e)
                {
                    std::cerr <<"Error in receive_from: "<< e.what() << '\n';
                }
                
                // TODO will be check later, it maybe debug here.
                // std::this_thread::sleep_for(std::chrono::milliseconds(50)); //1000hz
            }
        }
    };
} // namespace FDSC
