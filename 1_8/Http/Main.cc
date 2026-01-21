#include <iostream>
#include <string>
#include <memory>
#include "Socket.hpp"
#include "TcpServer.hpp"
#include "Logger.hpp"
#include "Http.hpp"

void Usage(std::string proc)
{
    std::cout << "Usage:" << proc << " +port" << std::endl;
}

// std::string TestHttp(std::string &requeststr)
// {
//     std::cout << "################################" << std::endl;
//     std::cout << requeststr << std::endl;
//     std::cout << "################################" << std::endl;

//     std::string html = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n"
//                        "<title>Hello World</title>\r\n</head>\r\n<body>\r\n"
//                        "<h1>hello, world</h1>\r\n</body>\r\n</html>";

//     std::string response = "HTTP/1.1 200 OK\r\n"; // 1. 这里是 OK 不是 0K
//     response += "Content-Type: text/html\r\n";
//     response += "Content-Length: " + std::to_string(html.size()) + "\r\n";
//     response += "\r\n"; // 2. 头部与正文之间必须空行
//     response += html;

//     return response;
// }

// 参数形式：./Server port
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
    }

    EnableConsoleLogStrategy();

    // 计算机对象
    u_int16_t serverport = std::stoi(argv[1]);
    //HTTP协议
    std::unique_ptr<Http> http=std::make_unique<Http>();

    std::unique_ptr<TcpServer> tsock = std::make_unique<TcpServer>(serverport,
         [&http](std::string &reqstr)->std::string 
         {return http->HandlerRequset(reqstr);});
    tsock->Run();
    
    return 0;
}

