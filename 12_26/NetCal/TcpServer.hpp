#pragma once

#include "Socket.hpp"
#include <memory>
#include <signal.h>
#include <functional>

using callback_t = std::function<std::string(std::string &)>; // 输入输出

class TcpServer
{
public:
    TcpServer(int port, callback_t cb)
        : _port(port),
          _cb(cb),
          _listensocket(std::make_unique<TcpSocket>())
    {
        _listensocket->BuildListenSocketMethod(_port);
    }

    void HandlerRequest(std::shared_ptr<Socket> sockfd, InetAddr addr)
    {
        // 长服务
        std::string inbuffer;
        while (true)
        {
            ssize_t n = sockfd->Recv(&inbuffer);
            if (n > 0)
            {
            }
            else if (n == 0)
            {
                LOG(LogLevel::DEBUG) << addr.ToString() << "quit,me too";
                break;
            }
            else
            {
                LOG(LogLevel::DEBUG) << addr.ToString() << "read error,quit";
            }
        }
    }

    void Run()
    {
        signal(SIGCHLD,SIG_IGN);
        while (true)
        {
            InetAddr addr;
            auto sockfd = _listensocket->Accept(&addr);
            if (sockfd == nullptr)
                continue;

            LOG(LogLevel::INFO) << "获取一个新连接" << addr.ToString() << " sockfd:" << sockfd;
        }
    }

    ~TcpServer()
    {
    }

private:
    int _port;
    std::unique_ptr<Socket> _listensocket;
    callback_t _cb;
};