#pragma once

#include <iostream>
#include <string>
#include <sys/select.h>
#include <memory>
#include <algorithm>
#include "Socket.hpp"
#include "Logger.hpp"
#include "InetAddr.hpp"

const static int gsize = sizeof(fd_set) * 8; // 最大存储文件描述符个数
const static int gdefaultfd = -1;            // 默认文件描述符值

class SelectServer
{
public:
    SelectServer(uint16_t port)
        : _listensock(std::make_unique<TcpSocket>())
    {
        // 创建监听套接字
        _listensock->BuildListenSocketMethod(port);
        // 初始化文件描述符集
        for (int i = 0; i < gsize; i++)
        {
            fd_array[i] = gdefaultfd;
        }
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
            if (fd_array[pos] == gdefaultfd) // 找到空位置
            {
                fd_array[pos] = sockfd; // 将新客户连接添加进去
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
        int sockfd = fd_array[index];
        char buffer[1024];
        ssize_t n = read(sockfd, buffer, sizeof(buffer) - 1); // 接收客户端发来的数据
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
            fd_array[index] = gdefaultfd;
            close(sockfd);
            LOG(LogLevel::INFO) << "client quit,me too" << fd_array[index];
        }
        else
        {
            // 读出错
            fd_array[index] = gdefaultfd;
            close(sockfd);
            LOG(LogLevel::WARNING) << "recv error" << fd_array[index];
        }
    }

    //事件派发器，不同事件派发到不同处理函数中
    void EventDispatcher(fd_set &rfds)
    {
        LOG(LogLevel::INFO) << "fd就绪，有新事件到来";

        for (int i = 0; i < gsize; i++)
        {
            if (fd_array[i] == gdefaultfd)
                continue;

            // 有读事件就绪
            if (FD_ISSET(fd_array[i], &rfds))
            {
                if (fd_array[i] == _listensock->Sockfd()) // 监听套接字就绪
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
            // int maxfd = _listensock->Sockfd();
            int maxfd = gdefaultfd; // 先设置初始值
            struct timeval timeout = {0, 0};

            fd_set rfds;    // 定义可读文件描述符集
            FD_ZERO(&rfds); // 清空可读描述符集
            // 循环遍历找到最大操作符
            for (int i = 0; i < gsize; i++)
            {
                if (fd_array[i] == gdefaultfd)
                    continue;

                // 第一次会将监听套接字文件描述符设置进去
                // 之后监听套接字文件描述符有消息即代表有新连接请求

                FD_SET(fd_array[i], &rfds); // 将有效文件描述符设置进去

                if (maxfd < fd_array[i])
                    maxfd = fd_array[i]; // 更新最大文件描述符
                LOG(LogLevel::INFO) << "添加fd：" << fd_array[i];
            }
            // FD_SET(_listensock->Sockfd(), &rfds);
            int n = select(maxfd + 1, &rfds, nullptr, nullptr, &timeout);
            switch (n)
            {
            case 0:
                LOG(LogLevel::DEBUG) << "timeout……" << timeout.tv_sec << ":" << timeout;
                break;
            case -1:
                LOG(LogLevel::ERROR) << "select error";
                break;
            default:
                EventDispatcher(rfds);
            }
        }
    }

    ~SelectServer();

private:
    std::unique_ptr<Socket> _listensock;
    int fd_array[gsize];
};
