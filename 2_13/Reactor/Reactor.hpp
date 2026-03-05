#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include "Connection.hpp"
#include "Socket.hpp"
#include "Epoller.hpp"

class Reactor
{
private:
    bool IsExist(std::shared_ptr<Connection>&conn)
    {
        auto iter = _connections.find(conn->Sockfd());
        return iter != _connections.end();
    }

public:
    Reactor() : _epoller(std::make_unique<Epoller>())
    {
    }

    // 从多路复用器（epoll）获取就绪的事件
    void AddConnection()
    {
        // 判断是否存在
        if (!IsExist())
        {
            // 不存在则插入
        }
    }

    ~Reactor()
    {
    }

private:
    std::unordered_map<int, std::shared_ptr<Connection>> _connections;
    std::unique_ptr<Epoller> _epoller;
};