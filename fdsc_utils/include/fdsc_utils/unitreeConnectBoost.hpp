#ifndef _FDSC_UNICONNECT_BOOST_H_
#define _FDSC_UNICONNECT_BOOST_H_

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
        UnitreeConnection(const std::string &settings = "HIGH_WIFI_DEFAULTS");
        ~UnitreeConnection();

        void startRecv();
        void stopRecv();
        void send(const std::vector<uint8_t> &cmd);
        std::vector<std::vector<uint8_t>> getData();

        void getData(std::vector<std::vector<uint8_t>> &ret);

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

        void initialize();
        void recvThread();
    };
} // namespace FDSC

#endif
