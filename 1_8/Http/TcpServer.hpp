#pragma once

#include "Socket.hpp"
#include "InetAddr.hpp"
#include <unistd.h>
#include <memory>
#include <signal.h>
#include <functional>

using callback_t = std::function<std::string(std::string &)>; // 输入输出

class TcpServer
{
public:
    TcpServer(int port, callback_t cb)
        : _port(port),
          _listensocket(std::make_unique<TcpSocket>()), // 创建监听套接字
          _cb(cb)                                       // 储存业务处理回调函数
    {
        _listensocket->BuildListenSocketMethod(_port); // 启动监听
    }

    void HandlerRequest(std::shared_ptr<Socket> sockfd, InetAddr addr)
    {
        // 短服务
        std::string inbuffer;

        ssize_t n = sockfd->Recv(&inbuffer); // 从客户端接收数据
        if (n > 0)
        {
            std::string send_str = _cb(inbuffer);
            // 回调函数：反序列化解析接收的数据 ->处理数据 ->序列化结果
                sockfd->Send(send_str); // 发送回客户端
        }

        else if (n == 0) // 客户端正常关闭
        {
            LOG(LogLevel::DEBUG) << addr.ToString() << "quit,me too";
        }
        else // 读取错误（n<0）
        {
            LOG(LogLevel::DEBUG) << addr.ToString() << "read error,quit";
        }

        // sockfd->Close();
    }

    void Run()
    {

        // 子进程执行完成后内核自动清理资源，不会变成僵尸进程
        while (true)
        {

            InetAddr addr; // 存储客户端地址

            auto sockfd = _listensocket->Accept(&addr); // 接受新连接

            if (sockfd == nullptr) // 连接失败
                continue;          // 等待下一个连接

            LOG(LogLevel::INFO) << "获取一个新连接" << addr.ToString() << " sockfd:" << sockfd;

            if (fork() == 0) // 创建子进程进行连接处理
            {
                _listensocket->Close();
                HandlerRequest(sockfd, addr);
                exit(0);
            }
            // sockfd->Close();
        }
    }

    ~TcpServer()
    {
    }

private:
    int _port;
    std::unique_ptr<Socket> _listensocket;
    callback_t _cb;
};