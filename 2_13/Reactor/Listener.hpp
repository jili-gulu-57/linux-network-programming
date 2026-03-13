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
        SetNonBlock(_listensockfd->Sockfd());
    }

    void Recever() override
    {
        LOG(LogLevel::INFO) << "事件派发成功，被回调到这里";
        while (true)
        {
            InetAddr clientaddr;
            int error = 0;
            int sockfd = _listensockfd->Accept(&clientaddr, &error);
            if (sockfd < 0)
            {
                if (errno == EAGAIN)
                    break;
                else if (errno == EINTR)
                    continue;
                else
                    break;
            }
            // 获取新连接
            // 1.设置非阻塞
            SetNonBlock(sockfd);
            std::shared_ptr<Connection> conn = std::make_shared<Channel>(sockfd, clientaddr);
            conn->SetCallback(_cb);
            // 2.添加到Reactor
            Owner()->AddConnection(conn);
        }
    }

    // 监听socket不需要处理读写事件
    void Sender() override
    { // empty
    }
    void Excepter() override
    { // empty
    }

    ~Listener()
    {
    }

private:
    std::unique_ptr<Socket> _listensockfd;
    uint16_t _port;
};