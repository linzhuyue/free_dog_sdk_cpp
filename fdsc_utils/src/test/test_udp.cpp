#include <bits/stdc++.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
   
#define PORT     8082
#define MAXLINE 2048 
void show_byte_data(const std::vector<uint8_t>& data, int datalen)
{
    int count = 0;
    for (auto packet : data) {
        std::cout <<"0x"<<std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(packet)<<", ";
        if (count % datalen==0 && count!=0)
        {
            std::cout<<std::endl;
        }
        
        count++;
    }
}
// Driver code 
int main() { 
    const std::string Add_r="192.168.123.161" ;
    int sockfd; 
    // char buffer[MAXLINE]; 
    const char *hello = "Hello from client"; 
    struct sockaddr_in     servaddr; 
   
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
   
    memset(&servaddr, 0, sizeof(servaddr)); 
       
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    // servaddr.sin_addr.s_addr = INADDR_ANY; 
    inet_pton(AF_INET, Add_r.c_str(), &(servaddr.sin_addr));
       
    int n;
    socklen_t len = sizeof(servaddr); 
       
    sendto(sockfd, (const char *)hello, strlen(hello), 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr)); 

    uint8_t buffer[2048];
    memset(buffer, 0, 2048);
    ssize_t bytesRead = recvfrom(sockfd, buffer, sizeof(buffer),  
                MSG_WAITALL, (struct sockaddr *) &servaddr, 
                &len); 
    std::vector<uint8_t> data_;
    data_.insert(data_.end(), buffer, buffer + bytesRead);
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); //wait all the data recv finished                
    // std::cout<<"Server :"<<buffer<<std::endl; 
    std::cout<<"datalen:" <<data_.size()<<std::endl;
    show_byte_data(data_,8);
    close(sockfd); 
    return 0; 
}