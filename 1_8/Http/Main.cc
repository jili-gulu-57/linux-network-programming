#include <iostream>
#include <string>
#include<memory>
#include "Socket.hpp"
#include "TcpServer.hpp"

void Usage(std::string proc)
{
    std::cout << "Usage:" << proc << " +port" << std::endl;
}

std::string TestHttp(std::string&requeststr )
{
    std::cout<<"################################"<<std::endl;
    std::cout<<requeststr<<std::endl;
    std::cout<<"################################"<<std::endl;

    std::string response="HTTP/1.1 200 OK\r\n\r\n";

    response+="<!DOCTYPE html>\r\n<html>\r\n<head>\r\n\
    <title>Hello World</title>\r\n</head>\r\n<body>\r\n\
    <h1>hello, world</h1>\r\n</body>\r\n</html>";

    return response;
}

// 参数形式：./Server port
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[1]);
    }

    EnableConsoleLogStrategy();

    //计算机对象

}