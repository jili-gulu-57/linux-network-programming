#pragma once

#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <strings.h>
#include <string>
#include "Logger.hpp"
#include "InetAddr.hpp"

using callback_t = std::is_function<void(int sockfd, std::string message, InetAddr addr)>;

static const int gdefaultsockfd = -1;

class ChatServer
{
public:
    ChatServer(const std::string &ip, u_int16_t port)
        : _sockfd(gdefaultsockfd),
          _port(port)
    {}

    // 类的定义内部只能包含成员变量和成员函数的声明或定义，不能直接执行语句（除非在函数内部或使用 C++11 的成员初始化）
    void Init()
    {
        // 创建套接字
        // 参数：AF_INET表示IPV4协议簇，SOCK_DGRAM表示UDP类型（面向数据段，无连接），0表示阻塞
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);

        // 创建失败，直接退出
        if (_sockfd < 0) // 创建失败返回-1
        {
            LOG(LogLevel::FATAL) << "create socket fail!";
            exit(1);
        }
        LOG(LogLevel::INFO) << "create socket success" << _sockfd; // 3

        // 2.bind
        // 2.1 填充ip和port
        // 绑定套接字到本地地址
        struct sockaddr_in local;     // IPv4地址结构体
        bzero(&local, sizeof(local)); // 将这一段内存空间清零，避免垃圾值

        void Usage(std::string proc)
        {
            std::cout << "Usage:" << proc << "serverip serverport" << std::endl;
        }

        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        // local.sin_addr.inet_addr(_ip.c_str());//ip地址字符串转网络字节序
        // 服务器有多个IP，客户端选择其中一个连接
        local.sin_addr.s_addr = htonl(INADDR_ANY); // bind任意IP

        // 2.2和socketfd进行bind
        int n = bind(_sockfd, (struct sockaddr *)&local, sizeof(local));
        // struct sockaddr为通用地址结构体

        if (n < 0) // bind失败返回-1
        {
            LOG(LogLevel::FATAL) << "bind socket error";
            exit(2);
        }
        LOG(LogLevel::INFO) << "bind socket success" << _sockfd;
    }

    void Start()
    {
        _is_running = true;
        while (_is_running)
        {
            // 用于接收数据的缓冲区
            char buffer[1024];
            buffer[0] = 0; // 清空缓冲区

            // 用于接收发送方的套接字
            struct sockaddr_in peer; // 存储客户端地址信息
            socklen_t len = sizeof(peer);

            // 用于无连接的套接字接收数据
            ssize_t n = recvfrom(_sockfd, buffer, sizeof(buffer), 0,
                                 (struct sockaddr *)&peer, &len);

            if (n > 0) // 接收到数据
            {
                // 手动添加字符串结束标识
                buffer[n] = 0;

                InetAddr clientaddr(peer);
                LOG(LogLevel::DEBUG)<<"get a client info#"
                <<clientaddr.IP()<<"-"<<clientaddr.PORT()<<":"<<buffer;

                std::string message = buffer;

                // 回调
                _cb(_sockfd, message, clientaddr);
            }
        }
        _is_running = false;
    }

    void Stop()
    {
        _is_running = false;
    }

    ~ChatServer()
    {
    }

private:
    int _sockfd;     // socket文件描述符
    u_int16_t _port; // 端口号（16位无符号整数）
    // std::string _ip;    //ip地址字符串
    bool _is_running; // 服务器运行状态标识符
    callback_t _cb;
};