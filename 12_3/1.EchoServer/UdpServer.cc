#include"UdpServer.hpp"
#include<iostream>
#include<memory>

void Usage(std::string proc)
{
    std::cout<<"Usage:"<<proc<<"serverip serverport"<<std::endl;
}

int main(int argc,char*argv[])
{
    if(argc!=3)
    {
        Usage(argv[0]);
        exit(1);
    }

    std::string ip=argv[1];
    u_int16_t port=std::stoi(argv[2]);

    EnableConsoleLogStrategy();
    std::unique_ptr<UdpServer> usvr=std::make_unique<UdpServer>();
    usvr->Init();
    
    return 0;
}