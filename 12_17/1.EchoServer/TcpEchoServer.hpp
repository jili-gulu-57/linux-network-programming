#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Mutex.hpp"
#include "Logger.hpp"
#include "Comm.hpp"
#include"InetAddr.hpp"

static const int gdefaultfd = -1;
static const int gbacklog = 8;
static const int gport = 8080;

class TcpEchoServer
{
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
            exit(SOCKET_CREATE_ERR);
        }
        LOG(LogLevel::INFO) << "create tcp socket success: " << _listensockfd;

        // 2.bind
        // sockaddr_in local;
        // memset(&local, 0, sizeof(local));
        // local.sin_port = htons(_port);
        // local.sin_family = AF_INET;
        // local.sin_addr.s_addr = htonl(INADDR_ANY);

        InetAddr local(_port);

        if (bind(_listensockfd, local.ADDR(), local.Length() != 0)) // bind失败
        {
            LOG(LogLevel::FATAL) << "socket bind error";
            exit(SOCKET_BIND_ERR);
        }
        // bind成功
        LOG(LogLevel::INFO) << "socket bind success: " << _listensockfd;

        // 3.listen
        if (listen(_listensockfd, gbacklog) != 0)
        {
            LOG(LogLevel::FATAL) << "socket listen error";
            exit(SOCKET_LISTEN_ERR);
        }
        LOG(LogLevel::INFO) << "socket listen success" << _listensockfd;
    }

    void Start()
    {
        while (true)
        {
            struct sockaddr_in peer;
            socklen_t len=sizeof(peer);
            int sockfd=accept(_listensockfd,(struct sockaddr*)&peer,&len);
            if(sockfd<0)
            {
                LOG(LogLevel::WARNING)<<"accept socket error";
                continue;
            }

            InetAddr clientaddr(peer);
            LOG(LogLevel::INFO)<<"获取链接成功,socket: "<<sockfd<<"client addr: "<<clientaddr.ToString();
        }
    }

    ~TcpEchoServer()
    {
    }

private:
    int _listensockfd;  //监听套接字
    u_int16_t _port;
};
