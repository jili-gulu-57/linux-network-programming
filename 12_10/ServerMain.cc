#include "ChatServer.hpp"
#include <iostream>
#include <memory>
#include <string.h>

void Usage(std::string proc)
{
    std::cout << "Usage:" << proc << "serverip serverport" << std::endl;
}

void chat(int sockfd, std::string message,InetAddr addr)
{
    LOG(LogLevel::DEBUG)<<"sockfd: "<<sockfd;
    LOG(LogLevel::DEBUG)<<"message: "<<message;
    LOG(LogLevel::DEBUG)<<"client info: "<<addr.IP()<<"-"<<addr.PORT();
    sendto(sockfd,message.c_str(),message.size(),0,addr.ADDR(),)
}
// ./
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }

    EnableConsoleLogStrategy();
    u_int16_t port = std::stoi(argv[1]);

    std::unique_ptr<ChatServer> usvr = std::make_unique<ChatServer>(port,chat);

    
    return 0;
}