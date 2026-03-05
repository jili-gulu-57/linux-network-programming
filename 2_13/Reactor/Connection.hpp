#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include "Socket.hpp"
#include "Reactor.hpp"

// 标识接收到的一条链接
class Connection
{
public:
    Connection()
    {
    }

    virtual void Recever() = 0;  // 处理读事件
    virtual void Sender() = 0;   // 处理写事件
    virtual void Excepter() = 0; // 处理异常事件

    int Sockfd()
    {
        return _sockfd;
    }

    virtual ~Connection()
    {
    }

private:
    int _sockfd;
    std::string _inbuffer;
    std::string _outbuffer;
    InetAddr peer;
    Reactor *owner;
};