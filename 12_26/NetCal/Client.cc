#include <iostream>
#include <string>
#include <memory>
#include "Socket.hpp"
#include "TcpServer.hpp"
#include "Parser.hpp"
#include "Protocol.hpp"

void Usage(std::string proc)
{
    std::cerr << "usage:" << proc << "serverip serverport" << std::endl;
}

// ./client serverip serverport
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(0);
    }

    std::string serverip = argv[1];
    u_int16_t serverport = std::stoi(argv[2]);
    std::unique_ptr<Socket> sockptr = std::make_unique<TcpSocket>();
    sockptr->BuildClientSocketMethod();
    InetAddr server(serverport,serverip);
    if(sockptr->Connect(server))
    {
        std::string inbuffer;
        while(true)
        {
            //1.构建请求
            Request req;
            std::cout<<"请输入x：";
            std::cin>>req._x;
            std::cout<<"请输入y：";
            std::cin>>req._y;
            std::cout<<"请输入操作：";
            std::cin>>req._oper;

            //2.序列化
            std::string jsonstr;
            req.Serialize(&jsonstr);

            //3.打包
            std::string send_str=Protocol::Package(jsonstr);

            //4.发送
            sockptr->Send(send_str);

            //5.接收
            sockptr->Recv(&inbuffer);

            //6.报文解析（①解包 ②反序列化）
            std::string package;
            Protocol::Unpack(inbuffer,&package);

            ResPonse resp;
            resp.Deserialize(package);
            resp.Print();
        }
    }
    return 0;
}