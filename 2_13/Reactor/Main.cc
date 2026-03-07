#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include "Socket.hpp"
#include "Reactor.hpp"
#include "Connection.hpp"

void Usage(std::string proc)
{
    std::cout << "Usage: "<<proc<<" + port" << std::endl;
}

int main(int argc,char* argv[])
{
    if(argc!=2)
    {
        Usage(argv[0]);
        exit(0);
    }

    //日志服务
    EnableConsoleLogStrategy();


    //网络服务
    uint16_t serverport = std::stoi(argv[1]);

    //连接管理器
    std::shared_ptr<Connection> listener = std::make_shared<listener>();

    //构建一个Reactor容器
    std::unique_ptr<Reactor>  R=std::make_unique<>


    return 0;
}