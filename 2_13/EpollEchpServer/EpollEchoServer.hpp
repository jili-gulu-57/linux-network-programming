#pragma once

#include <iostream>
#include <sys/epoll.h>
#include <string>
#include <memory>
#include "Socket.hpp"
#include "Logger.hpp"

static const int gsize = 64;

class EpollServer
{
public:
    EpollServer(uint16_t port)
        : _listensock(std::make_unique<TcpSocket>())
    {
        _listensock->BuildListenSocketMethod(port);
        _epfd = epoll_create(128); // 内核2.6.8之后参数被省略，但需传入一个正整数
        if (_epfd < 0)
        {
            LOG(LogLevel::FATAL) << "epoll create fail!";
            return;
        }
        LOG(LogLevel::INFO) << "listen sock is:" << _listensock->Sockfd() << "epoll fd is:" << _epfd;
        struct epoll_event ev;
        ev.data.fd = _listensock->Sockfd();
        ev.events = EPOLLIN;

        int n = epoll_ctl(_epfd, EPOLL_CTL_ADD, _listensock->Sockfd(), &ev);
        (void)n;
    }

    void Start()
    {
        int timeout = 1000;
        while (true)
        {
            int n = epoll_wait(_epfd, revs, gsize, timeout);
            switch (n)
            {
            case 0:
                LOG(LogLevel::DEBUG) << "time out...";
                break;
            case -1:
                LOG(LogLevel::FATAL) << "epoll error";
            default:
                break;
            }
        }
    }

    ~EpollServer()
    {
    }

private:
    std::unique_ptr<Socket> _listensock;
    int _epfd;                      // epoll_create返回值
    struct epoll_event revs[gsize]; // return events
};
