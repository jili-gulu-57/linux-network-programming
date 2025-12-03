#pragma once

#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include<arpa/inet.h>
#include <string>
#include "Logger.hpp"

static const int gdefaultsockfd = -1;

class UdpServer
{
public:
    UdpServer(const std::string &ip, u_int16_t port)
        : _ip(ip),
          _port(port),
          _sockfd(gdefaultsockfd)
    {
    }
    void Init()
    {
        // 创建套接字
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        // 创建失败，直接退出
        if (_sockfd < 0)
        {
            LOG(LogLevel::FATAL) << "create socket fail!";
            exit(1);
        }
        LOG(LogLevel::INFO)<<"create socket success"<<_sockfd; //3
    }

    void Start()
    {
        
    }
    ~UdpServer()
    {}

private:
    int _sockfd;
    std::string _ip;
    u_int16_t _port;
};