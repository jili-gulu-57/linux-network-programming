#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "CommandServer.hpp"

void Usage(std::string proc)
{
    std::cerr << "Usage:" << proc << "serverip serverport" << std::endl;
}

// ./server ip port
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(0);
    }

    // 获取服务器端套接字
    std::string serverip = argv[1];
    u_int16_t serverport = std::stoi(argv[2]);

    // 创建客户端套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "create client socket error" << std::endl;
        exit(SOCKET_CREATE_ERR);
    }
    // tcp客户端不需要显示bind，os会随机bind一个端口号

    //准备服务器地址信息
    InetAddr server(serverport, serverip);

    // 连接服务器
    if (connect(sockfd, server.Addr(), server.Length()) < 0)
    {
        std::cerr << "connect server error" << std::endl;
        exit(SOCKET_CONNECT_ERR);
    }
    std::cout << "connect server: " << server.ToString() << "success " << std::endl;

    //主循环：与服务器进行交互
    while (true)
    {
        // 客户端输入
        std::cout << "Please enter: ";
        std::string line;
        std::getline(std::cin, line);   //读取整行输入

        // 发送数据到服务器
        ssize_t n = write(sockfd, line.c_str(), line.size());

        if (n > 0)  //发送成功
        {
            char buffer[1024];  //准备缓冲区
            ssize_t m = read(sockfd, buffer, sizeof(buffer) - 1);//预留一个字符位置给字符串结束符

            //服务器成功接收响应
            if (m > 0)
            {
                buffer[m] = 0;
                std::cout << buffer << std::endl;
            }
        }
    }

    close(sockfd);

    return 0;
}
