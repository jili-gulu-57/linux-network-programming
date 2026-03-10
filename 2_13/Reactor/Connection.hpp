#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include "Socket.hpp"
#include "Reactor.hpp"

class Reactor;

using callback_t = std::function<std::string(std::string &inbuffer)>;

// 定义一个连接的抽象骨架，具体读写方法由子类实现
//  标识接收到的一条链接
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

    // 获取设置socket
    int Sockfd()
    {
        return _sockfd;
    }
    void SetSocketfd(int sockfd)
    {
        _sockfd = sockfd;
    }

    // 设置事件并同步到内核
    void SetEvents(uint32_t events)
    {
        _events = events;
    }
    // 获取事件
    uint32_t Events()
    {
        return _events;
    }

    // 地址信息
    void SetAddr(const InetAddr &addr)
    {
        _peer = addr;
    }

    // 所有者
    Reactor *Owner()
    {
        return _owner;
    }
    void SetOwner(Reactor *r)
    {
        _owner = r;
    }

    void SetCallback(callback_t cb)
    {
        _cb = cb;
    }

    virtual ~Connection()
    {
    }

protected:
    int _sockfd;
    uint32_t _events;
    std::string _inbuffer;
    std::string _outbuffer;

    InetAddr _peer;
    Reactor *_owner;
    callback_t _cb;
};