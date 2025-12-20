#include<iostream>
#include<memory>
#include"TcpEchoServer.hpp"

int main()
{
    EnableConsoleLogStrategy();

    std::unique_ptr<TcpEchoServer> tsvr=std::make_unique<TcpEchoServer>();

    tsvr->Init();
    tsvr->Start();
    return 0;
}