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
    static const int size = 128;

private:
    bool IsExist(std::shared_ptr<Connection> &conn)
    {
        auto iter = _connections.find(conn->Sockfd());
        return iter != _connections.end();
    }

    bool IsExist(int sockfd)
    {
        auto iter = _connections.find(sockfd);
        return iter != _connections.end();
    }

public:
    Reactor() : _epoller(std::make_unique<Epoller>())
    {
    }

    // 从多路复用器（epoll）获取就绪的事件
    void AddConnection(std::shared_ptr<Connection> &conn)
    {
        // 判断是否存在
        if (IsExist(conn))
        {
            LOG(LogLevel::INFO) << conn->Sockfd() << "conn in Reactor";
        }
        _connections.insert(std::make_pair(conn->Sockfd(), conn));
        _epoller->AddEvent(conn->Sockfd(), conn->Events());
        LOG(LogLevel::INFO) << conn->Sockfd() << "conn add to Reactor";
    }

    void LoopOnce(int timeout)
    {
        int n = _epoller->Wait(revs, size, timeout);
        for (int i = 0; i < n; i++)
        {
            int sockfd = revs[i].data.fd;
            uint32_t events = revs[i].events;

            // 统一报错的处理方法
            if (events & EPOLLHUP)
                events = (EPOLLIN | EPOLLOUT);
            if (events & EPOLLERR)
                events = (EPOLLIN | EPOLLOUT);

            if ((events & EPOLLIN) && IsExist(sockfd))
            {
                _connections[sockfd]->Recever();
            }
        }
    }

    void Dispatcher()
    {
        int timeout = 1000;
        while (true)
        {
            // 处理事件
            LoopOnce(timeout);
        }
    }

    ~Reactor()
    {
    }

private:
    std::unordered_map<int, std::shared_ptr<Connection>> _connections;
    std::unique_ptr<Epoller> _epoller;
    struct epoll_event revs[size];
};