#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include "Socket.hpp"
#include "TcpServer.hpp"
#include "Connection.hpp"

class Listener : public Connection
{
public:
    Listener()
    {
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

    ~Listener()
    {
    }
};