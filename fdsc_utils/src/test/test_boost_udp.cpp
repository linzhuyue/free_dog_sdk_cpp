#include <iostream>
#include <boost/asio.hpp>
#include <memory>
#include <thread>

const int PORT = 12345;

void receiveData(std::shared_ptr<boost::asio::ip::udp::socket> socket) {
    try {
        boost::asio::ip::udp::endpoint senderEndpoint;

        while (true) {
            std::array<char, 128> recvBuffer;
            boost::system::error_code error;

            size_t bytesRead = socket->receive_from(boost::asio::buffer(recvBuffer), senderEndpoint, 0, error);

            if (error && error != boost::asio::error::message_size) {
                throw boost::system::system_error(error);
            }

            std::string receivedMessage(recvBuffer.data(), bytesRead);
            std::cout << "Received: " << receivedMessage << std::endl;
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

int main() {
    boost::asio::io_context ioContext;
    std::shared_ptr<boost::asio::ip::udp::socket> socket = std::make_shared<boost::asio::ip::udp::socket>(ioContext);

    try {
        socket->open(boost::asio::ip::udp::v4());

        std::thread receiverThread(receiveData, socket);
        receiverThread.detach(); // Detach the receiver thread

        boost::asio::ip::udp::endpoint receiverEndpoint(boost::asio::ip::address::from_string("127.0.0.1"), PORT);

        while (true) {
            std::string message;
            std::cout << "Enter a message to send (or type 'exit' to quit): ";
            std::cin >> message;

            if (message == "exit") {
                break;
            }

            socket->send_to(boost::asio::buffer(message), receiverEndpoint);
        }

        ioContext.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
