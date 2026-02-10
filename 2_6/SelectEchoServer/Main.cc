#include <iostream>
#include <string>
#include <memory>
#include "SelectServer.hpp"

void Usage(std::string proc)
{
    std::cout << "Usage: " << proc << " + localport" << std::endl;
}

int main(int argc, char *argv[]) // char *argv[]字符串数组
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(0);
    }

    uint16_t serverport = std::stoi(argv[1]);

    EnableConsoleLogStrategy();

    std::unique_ptr<SelectServer> selectsvr = std::make_unique<SelectServer>(serverport);
    selectsvr->Run();
    
    return 0;
}