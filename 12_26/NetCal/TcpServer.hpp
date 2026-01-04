#pragma once

#include "Socket.hpp"
#include <memory>

class TcpServer
{
public:
    TcpServer(int port)
        : _port(port),
          _listensocket(std::make_unique<TcpSocket>())
    {
        _listensocket->BuildListenSocketMethod(_port);
    }

    void Run()
    {
        while (true)
        {
            InetAddr addr;
            auto sockfd = _listensocket->Accept(&addr);
            if (sockfd == nullptr)
                continue;

            LOG(LogLevel::INFO) << "获取一个新连接" << addr.ToString() << " sockfd:" << sockfd;
        }
    }

    void HandlerRequest(std::shared_ptr<Socket>sockfd,InetAddr addr)
    {
        //长服务
        std::string inbuffer;
        while(true)
        {
            ssize_t n=sockfd->Recv(&inbuffer);
            if(n>0)
            {

            }
            else if(n==0)
            {
                LOG(LogLevel::DEBUG)<<addr.ToString()<<"quit,me too";
                break;
            }
            else
            {
                LOG(LogLevel::DEBUG)<<addr.ToString()<<"read error,quit";
            }
        }
    }

    ~TcpServer()
    {
    }

private:
    int _port;
    std::unique_ptr<Socket> _listensocket;
};