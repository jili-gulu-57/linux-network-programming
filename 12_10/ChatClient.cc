#include "ChatServer.hpp"
#include <iostream>
#include <memory>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include <string.h>
#include<thread>    //C++多线程

int sockfd=-1;

void Usage(std::string proc)
{
    std::cout << "Usage:" << proc << "serverip serverport" << std::endl;
}

void InitClient(const std::string&serverip,u_int16_t serverport)
{
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0)
    {
        std::cout<<"create sockfd error"<<std::endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }

    std::string serverip = argv[1];
    u_int16_t serverport = std::stoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cout << "create socket fail" << std::endl;
        return 0;
    }

    // 客户端不需要显示bindIP和端口号，OS会随机bind端口号

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(serverport);
    server.sin_addr.s_addr = inet_addr(serverip.c_str());

    while (true)
    {
        std::cout << "Please enter: " << std::endl;
        std::string line;
        std::getline(std::cin, line);

        // 写
        sendto(sockfd, line.c_str(), line.size(), 0, (struct sockaddr *)&server, sizeof(server));

        // 读
        struct sockaddr_in temp;
        socklen_t len = sizeof(temp);
        char buffer[1024];
        int m = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&temp, &len);
        if (m > 0)
        {
            buffer[m]=0;
            std::cout<<buffer<<std::endl;
        }
    }

    EnableConsoleLogStrategy();
    std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>();
    usvr->Init();

    return 0;
}