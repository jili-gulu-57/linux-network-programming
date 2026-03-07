#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include "Socket.hpp"
#include "InetAddr.hpp"
#include "Reactor.hpp"
#include "Connection.hpp"
#include "Util.hpp"
#include "Channel.hpp"

// 连接管理器，创建监听套接字，获取新连接
class Listener : public Connection
{
public:
    Listener(uint16_t port)
        : _listensockfd(std::make_unique<TcpSocket>()),
          _port(port)
    {
        _listensockfd->BuildListenSocketMethod(_port);
        SetSocketfd(_listensockfd->Sockfd());
        SetEvents(EPOLLIN | EPOLLET);
    }

    void Recever() override
    {
        LOG(LogLevel::INFO) << "事件派发成功，被回调到这里";
        while (true)
        {
            InetAddr clientaddr;
            int sockfd = _listensockfd->Accept(&clientaddr);
            if (sockfd < 0)
            {
                if (errno == EAGAIN)
                    break;
                else if (errno == EINTR)
                    continue;
                else
                    break;
            }
            //获取新连接
            //1.设置非阻塞
            SetNonBlock(sockfd);
            std::shared_ptr<Connection> conn = std::make_shared<Channel>(sockfd, clientaddr);
            //2.添加到Reactor
            
        }
    }
    void Sender() override
    {
    }
    void Excepter() override
    {
    }

    ~Listener()
    {
    }

private:
    std::unique_ptr<Socket> _listensockfd;
    uint16_t _port;
};