#pragma once

#include <iostream>
#include <string>
#include <poll.h>
#include <memory>
#include <algorithm>
#include "Socket.hpp"
#include "Logger.hpp"
#include "InetAddr.hpp"

const static int gsize = sizeof(fd_set) * 8; // 最大存储文件描述符个数
const static int gdefaultfd = -1;            // 默认文件描述符值

class PollServer
{
public:
    PollServer(uint16_t port)
        : _listensock(std::make_unique<TcpSocket>())
    {
        // 创建监听套接字
        _listensock->BuildListenSocketMethod(port);
        // 初始化文件描述符集
        for (int i = 0; i < gsize; i++)
        {
            fd_array[i].fd = gdefaultfd;
            fd_array[i].events = fd_array[i].revents = 0;
        }
        fd_array[0].fd = _listensock->Sockfd();
        fd_array[0].events = POLLIN;
    }

    // 接收客户端请求
    void Accepter()
    {
        // 获取新连接，并保存
        InetAddr clientaddr;
        int sockfd = _listensock->Accept(&clientaddr);
        if (sockfd > 0)
        {
            LOG(LogLevel::INFO) << "get new socket:" << sockfd << ",clienaddr:" << clientaddr.ToString();
        }

        // 在可读文件描述符集中寻找空位置
        int pos = 0;
        for (; pos < gsize; pos++)
        {
            if (fd_array[pos].fd == gdefaultfd) // 找到空位置
            {
                fd_array[pos].fd = sockfd; // 将新客户连接添加进去
                fd_array[pos].events = POLLIN | POLLOUT;
                break;
            }
        }

        // 遍历到最后没有发现空位置，说明此时服务器已满
        if (pos == gsize)
        {
            LOG(LogLevel::WARNING) << "server is full!";
            close(sockfd);
        }
    }

    // 读、写暂时一起处理
    void Recver(int index)
    {
        int sockfd = fd_array[index].fd;
        char buffer[1024];
        ssize_t n = recv(sockfd, buffer, sizeof(buffer) - 1, 0); // 接收客户端发来的数据
        if (n > 0)
        {
            buffer[n] = 0;
            std::cout << "client say:" << buffer << std::endl; // 显示客户端发来的数据

            // 服务器回显数据
            std::string echo_string = "server echo:";
            echo_string += buffer;
            send(sockfd, echo_string.c_str(), sizeof(echo_string), 0);
        }
        else if (n == 0) // 客户端关闭
        {
            fd_array[index].fd = gdefaultfd;
            fd_array[index].events = fd_array[index].revents = 0;
            close(sockfd);
            LOG(LogLevel::INFO) << "client quit,me too" << fd_array[index].fd;
        }
        else
        {
            // 读出错
            fd_array[index].fd = gdefaultfd;
            fd_array[index].events = fd_array[index].revents = 0;
            close(sockfd);
            LOG(LogLevel::WARNING) << "recv error" << fd_array[index].fd;
        }
    }

    // 事件派发器，不同事件派发到不同处理函数中
    void EventDispatcher()
    {
        LOG(LogLevel::INFO) << "有新事件到来";

        for (int i = 0; i < gsize; i++)
        {
            if (fd_array[i].fd == gdefaultfd)
                continue;

            // 有读事件就绪
            if (fd_array[i].revents & POLLIN)
            {
                if (fd_array[i].fd == _listensock->Sockfd()) // 监听套接字就绪
                {
                    // 就绪说明有新连接到来，调用Accepter函数
                    Accepter();
                }

                else // 普通套接字就绪
                {
                    Recver(i);
                }
            }
        }
    }

    void Run()
    {
        while (true)
        {
            int timeout = 2000;
            int n = poll(fd_array, gsize, timeout);
            switch (n)
            {
            case 0:
                LOG(LogLevel::DEBUG) << "timeout……";
                break;
            case -1:
                LOG(LogLevel::ERROR) << "select error";
                break;
            default:
                EventDispatcher();
            }
        }
    }

    ~PollServer()
    {
    }

private:
    std::unique_ptr<Socket> _listensock;
    struct pollfd fd_array[gsize];
};
