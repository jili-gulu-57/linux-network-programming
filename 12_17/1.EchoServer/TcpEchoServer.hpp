#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Mutex.hpp"
#include "Logger.hpp"
#include "Comm.hpp"
#include "InetAddr.hpp"

static const int gdefaultfd = -1;
static const int gbacklog = 8;
static const int gport = 8080;

class TcpEchoServer
{
private:
    // 处理IO函数
    void HandlerIO(int sockfd, InetAddr &client)
    {
        char buffer[1024];
        while (true) // 循环读取客户端数据
        {
            buffer[0] = 0; // 清空缓冲区
            // 从socket读取数据，返回读取的字节数
            ssize_t n = read(sockfd, buffer, sizeof(buffer));
            if (n > 0) // 成功读取到数据
            {
                buffer[n] = 0; // 添加字符串结束符，保证是正确的c字符串
                std::string echo_string = "server echo: ";
                echo_string += buffer;

                //字符串回显到服务器端
                LOG(LogLevel::INFO)<<"client say: "<<buffer;
                
                // 将字符串回显到客户端
                write(sockfd, echo_string.c_str(), echo_string.size());
            }
            // 用户退出：EOF
            else if (n == 0)
            {
                LOG(LogLevel::INFO) << "client " << client.ToString() << "quit";
                break;
            }
            // 读取发生错误
            else
            {
                LOG(LogLevel::WARNING) << "read client " << client.ToString()
                                       << "error,sockfd:" << sockfd;
                break;
            }
        }
    }

public:
    TcpEchoServer(u_int16_t port = gport)
        : _listensockfd(gdefaultfd),
          _port(port)
    {
    }

    void Init()
    {
        // 1.创建套接字
        _listensockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (_listensockfd < 0) // 创建失败
        {
            LOG(LogLevel::ERROR) << "socket create error";
            exit(SOCKET_CREATE_ERR); // 程序退出
        }
        LOG(LogLevel::INFO) << "create tcp socket success: " << _listensockfd;

        // 2.bind
        // sockaddr_in local;
        // memset(&local, 0, sizeof(local));
        // local.sin_port = htons(_port);
        // local.sin_family = AF_INET;
        // local.sin_addr.s_addr = htonl(INADDR_ANY);

        InetAddr local(_port,"0.0.0.0");

        if (bind(_listensockfd, local.Addr(), local.Length()) != 0) // bind失败
        {
            LOG(LogLevel::FATAL) << "socket bind error,port: " << _port;

            struct sockaddr_in addr_info;
            socklen_t len = sizeof(addr_info);
            if (getsockname(_listensockfd, (struct sockaddr *)&addr_info, &len) == 0)
            {
                LOG(LogLevel::FATAL) << "Socket info - IP: "
                                     << inet_ntoa(addr_info.sin_addr)
                                     << ", Port: " << ntohs(addr_info.sin_port);
            }

            exit(SOCKET_BIND_ERR);
        }
        // bind成功
        LOG(LogLevel::INFO) << "socket bind success: " << _listensockfd;

        // 3.listen（开始监听）
        if (listen(_listensockfd, gbacklog) != 0)
        {
            LOG(LogLevel::FATAL) << "socket listen error";
            exit(SOCKET_LISTEN_ERR);
        }
        LOG(LogLevel::INFO) << "socket listen success" << _listensockfd;
    }

    void Start()
    {
        while (true) // 死循环，持续接收连接
        {
            struct sockaddr_in peer; // 存储客户端地址信息
            socklen_t len = sizeof(peer);
            // 接收客户端连接，返回新的文件描述符
            int sockfd = accept(_listensockfd, (struct sockaddr *)&peer, &len);

            if (sockfd < 0) // 连接失败
            {
                LOG(LogLevel::WARNING) << "accept socket error";
                continue; // 等待下一个连接
            }

            // 封装客户端地址
            InetAddr clientaddr(peer);
            LOG(LogLevel::INFO) << "获取链接成功,socket: " << sockfd
                                << "client addr: " << clientaddr.ToString();

            HandlerIO(sockfd, clientaddr);
        }
    }

    ~TcpEchoServer()
    {
    }

private:
    int _listensockfd; // 监听套接字
    u_int16_t _port;
};
