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
        : _events(0), _owner(nullptr)
    {
    }

    virtual void Recever() = 0;  // 处理读事件
    virtual void Sender() = 0;   // 处理写事件
    virtual void Excepter() = 0; // 处理异常事件

    int Sockfd()
    {
        return _sockfd;
    }

    void SetSocketfd(int sockfd)
    {
        _sockfd = sockfd;
    }

    void SetEvents(uint32_t events)
    {
        _events = events;
    }

    uint32_t Events()
    {
        return _events;
    }

    void SetAddr(const InetAddr &addr)
    {
        _peer = addr;
    }

    virtual ~Connection()
    {
    }

    Reactor *Owner()
    {
        return _owner;
    }

    void SetOwner(Reactor *r)
    {
        _owner = r;
    }

protected:
    int _sockfd;

private:
    uint32_t _events;
    std::string _inbuffer;
    std::string _outbuffer;

    InetAddr _peer;
    Reactor *_owner;
};