#include "Socket.hpp"
#include "TcpServer.hpp"
#include"Protocol.hpp"
#include <memory>

void Usage(std::string proc)
{
    std::cout << "Usage:" << proc << "localport" << std::endl;
}

int main(int argc,char* argv[])
{
    if(argc!=2)
    {
        Usage(argv[0]);
        exit(0);
    }

    u_int16_t serverport=std::stoi(argv[1]);

    EnableConsoleLogStrategy();

    std::unique_ptr<TcpServer> tsock=std::make_unique<TcpServer>(serverport);
    tsock->Run();

    return 0;
}