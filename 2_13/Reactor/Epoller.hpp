#pragma once

#include <iostream>
#include <string>
#include <sys/epoll.h>
#include <memory>
#include "Socket.hpp"
#include "Logger.hpp"

// 对epoll机制的简单封装
class Epoller
{
public:
    Epoller() // 构造函数，创建epoll实例
    {
        _epfd = epoll_create(128); // 需传入一个正整数
        if (_epfd < 0)
        {
            LOG(LogLevel::FATAL) << "epoll create fail!";
            exit(1);
        }
        LOG(LogLevel::INFO) << "epoll create succeed!";
    }

    // 添加要监听的文件描述符及事件（读、写等）
    void AddEvent(int sockfd, u_int32_t events)
    {
        struct epoll_event ev;
        ev.events = events;
        ev.data.fd = sockfd;
        int n = epoll_ctl(_epfd, EPOLL_CTL_ADD, sockfd, &ev);
        if (n != 0)
        {
            LOG(LogLevel::INFO) << sockfd << "：添加监听失败";
            return;
        }
        LOG(LogLevel::INFO) << sockfd << "：添加监听成功";
    }

    void DelEvent(int sockfd)
    {
        int n = epoll_ctl(_epfd, EPOLL_CTL_DEL, sockfd, nullptr);
        if (n != 0) // 删除失败
        {
            LOG(LogLevel::INFO) << "delete sockfd:" << sockfd << "fail!";
            return;
        }
        LOG(LogLevel::INFO) << "delete sockfd:" << sockfd << "success!";
    }

    void ModEvent(int sockfd, u_int32_t event)
    {
    }

    // 阻塞等待事件
    int Wait(struct epoll_event revs[], int num, int timeout)
    {
        int n = epoll_wait(_epfd, revs, num, timeout);
        // 返回值 n>0：就绪事件的数量 =0：timeout内无就绪事件 -1：出错
        (void)n;
        return n;
    }

    ~Epoller()
    {
        if (_epfd >= 0)
            close(_epfd);
    }

private:
    int _epfd = -1; // 后续管理文件描述符的句柄(初始化一个无效值)
};