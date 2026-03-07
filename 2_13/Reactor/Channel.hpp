#pragma once

#include <iostream>
#include <string>
#include "Connection.hpp"
#include "Listener.hpp"
#include "InetAddr.hpp"

class Channel : public Connection
{
public:
Channel(int sockfd, InetAddr &addr)
    {
        _sockfd = sockfd;
        SetAddr(addr);
        SetEvents(EPOLLIN | EPOLLET);
    }

    void Recever() override
    {
    }
    void Sender() override
    {
    }
    void Excepter() override
    {
    }

    ~Channel()
    {
    }

private:
};