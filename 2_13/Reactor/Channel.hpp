#pragma once

#include <iostream>
#include <string>
#include <sys/socket.h>
#include "Connection.hpp"
#include "Listener.hpp"
#include "InetAddr.hpp"

class Channel : public Connection
{
    static const int buffersize = 1024;

public:
    Channel(int sockfd, InetAddr &addr)
    {
        _sockfd = sockfd;
        SetAddr(addr);
        SetEvents(EPOLLIN | EPOLLET); // 默认监听读事件，边缘触发
    }

    void Recever() override
    {
        while (true)
        {
            char buffer[buffersize];
            ssize_t n = recv(_sockfd, buffer, sizeof(buffer) - 1, 0);
            if (n > 0)
            {
                buffer[n] = 0;
                _inbuffer += buffer; // 追加读
            }
            else if (n == 0)
            {
                LOG(LogLevel::INFO) << "client quit";
                Excepter();
                return; // 退出循环读
            }
            else
            {
                if (errno == EAGAIN) // 数据读完
                    break;
                else if (errno == EINTR) // 被中断
                    continue;
                else
                {
                    LOG(LogLevel::INFO) << "recv error";
                    Excepter();
                    return;
                }
            }
        }
        _outbuffer += _cb(_inbuffer);
        std::cout << "_outbuffer：" << _outbuffer << std::endl;
        if (!_outbuffer.empty())
        {
            Sender();
        }
    }
    void Sender() override
    {
        while (true)
        {
            ssize_t n = send(_sockfd, _outbuffer.c_str(), _outbuffer.size(), 0);
            if (n > 0)
            {
                _outbuffer.erase(0, n);
            }
        }
    }
    void Excepter() override
    {
    }

    ~Channel()
    {
    }

private:
};