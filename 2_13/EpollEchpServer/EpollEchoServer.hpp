#pragma once

#include <iostream>
#include <sys/epoll.h>
#include <string>
#include <memory>
#include "Socket.hpp"
#include "Logger.hpp"

class EpollServer
{
public:
    EpollServer()
    {

    }

    void Start()
    {

    }

    ~EpollServer()
    {

    }

private:
    std::unique_ptr<Socket> _listensock;
};
