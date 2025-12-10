#include "ChatServer.hpp"
#include <iostream>
#include <memory>
#include <string.h>

void Usage(std::string proc)
{
    std::cout << "Usage:" << proc << "serverip serverport" << std::endl;
}

std::string chat(const std::string &word, const std::string &whoip, const std::string &whoport)
{
    return "1";
}

// ./
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }

    EnableConsoleLogStrategy();
    u_int16_t serverport = std::stoi(argv[2]);

    std::unique_ptr<ChatServer> usvr = std::make_unique<ChatServer>(port,chat);

    
    return 0;
}