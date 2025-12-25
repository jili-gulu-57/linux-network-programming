#include<iostream>
#include<memory>
#include"TcpEchoServer.hpp"

int main()
{
    EnableConsoleLogStrategy();

    std::unique_ptr<TcpEchoServer> tsvr=std::make_unique<TcpEchoServer>();

    //初始化服务器：创建socket，完成bind，开始listen
    tsvr->Init();

    //启动服务器主循环：接收连接，进行IO处理
    tsvr->Start();
    return 0;
}