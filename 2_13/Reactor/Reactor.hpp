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
    // 辅助函数，判断连接是否存在
    bool IsExist(std::shared_ptr<Connection> &conn)
    {
        auto iter = _connections.find(conn->Sockfd());
        return iter != _connections.end();
    }
    // 函数重载
    bool IsExist(int sockfd)
    {
        auto iter = _connections.find(sockfd);
        return iter != _connections.end();
    }

public:
    Reactor() : _epoller(std::make_unique<Epoller>())
    {
    }

    // 将新连接注册到map和epoll中
    void AddConnection(std::shared_ptr<Connection> &conn)
    {
        // 1.判断是否存在
        if (IsExist(conn))
        {
            LOG(LogLevel::INFO) << conn->Sockfd() << "conn in Reactor";
            return;
        }
        conn->SetOwner(this);
        // 2.加入管理地图
        _connections.insert(std::make_pair(conn->Sockfd(), conn));
        // 3.注册epoll内核
        _epoller->AddEvent(conn->Sockfd(), conn->Events());
        LOG(LogLevel::INFO) << conn->Sockfd() << "conn add to Reactor";
    }

    // 执行一次“等待事件 -> 获取就绪事件 -> 预处理错误 -> 分发处理”
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

    // 事件分发器
    void Dispatcher()
    {
        int timeout = 1000; // 超时时间1000ms（1s）
        // 循环处理
        while (true)
        {
            // 处理事件
            LoopOnce(timeout);
        }
    }

    ~Reactor()
    {
        // unique_ptr和shared_ptr会自动清理资源
    }

private:
    std::unordered_map<int, std::shared_ptr<Connection>> _connections; // 资源管理器（维护所有活跃链接的映射表）
    std::unique_ptr<Epoller> _epoller;                                 // 事件监听器（封装了epoll）
    struct epoll_event revs[size];                                     // 事件就绪数组
};