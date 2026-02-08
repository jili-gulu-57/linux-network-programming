#pragma once

#include <iostream>
#include <sys/select.h>
#include <memory>
#include "Socket.hpp"
#include "Logger.hpp"
#include "TcpServer.hpp"

const static int gsize = sizeof(fd_set) * 8;    //最大存储文件描述符个数
const static int gdefaultfd = -1;   //默认文件描述符值

class SelectServer
{
public:
    SelectServer(uint16_t port)
        : _listensock(std::make_unique<TcpServer>())
    {
        _listensock->Bu
    }

    void HandleEvents()
    {
    }

    ~SelectServer();

private:
    std::unique_ptr<Socket> _listensock;
};
