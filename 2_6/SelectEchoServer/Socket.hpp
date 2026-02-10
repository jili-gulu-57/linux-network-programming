#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include "InetAddr.hpp"
#include "Logger.hpp"

enum
{
    OK,
    CREATE_ERR,
    BIND_ERR,
    LISTEN_ERR
};

static const int gsockfd = -1;
static int gbacklog = 16;

// 抽象基类，定义标准接口
// 父类控制整体流程，子类负责具体步骤
class Socket
{
public:
    virtual ~Socket() {} // 虚析构函数，确保派生类函数正确释放资源

    // 纯虚函数，定义socket操作接口
    //=0表示纯虚函数，派生类必须实现
    virtual void CreateSocketOrDie() = 0;
    virtual void BindSocketOrDie(u_int16_t port) = 0; // virtual表示这个函数可以在派生类中被重写
    virtual void ListenSocketOrDie(int backlog) = 0;
    // virtual std::shared_ptr<Socket> Accept(InetAddr *clientaddr) = 0;
    virtual int Accept(InetAddr *clientaddr) = 0;
    virtual int Sockfd() = 0;
    virtual void Close() = 0;
    virtual ssize_t Recv(std::string *out) = 0;
    virtual ssize_t Send(const std::string &in) = 0;
    virtual bool Connect(InetAddr &peer) = 0;

public:
    // 模板方法，组合了socket的完整建立过程
    void BuildListenSocketMethod(int _port)
    {
        CreateSocketOrDie();
        BindSocketOrDie(_port);
        ListenSocketOrDie(gbacklog);
    }

    void BuildClientSocketMethod()
    {
        CreateSocketOrDie();
    }
};

// 具体实现类（继承自socket类）
class TcpSocket : public Socket
{
public:
    TcpSocket() : _sockfd(gsockfd)
    {
    }

    TcpSocket(int sockfd) : _sockfd(sockfd)
    {
    }

    // 必须实现所有虚函数
    void CreateSocketOrDie() override // override：c++11关键字，表示正在重写虚函数
    {
        _sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (_sockfd < 0)
        {
            LOG(LogLevel::FATAL) << "create socket error";
            exit(CREATE_ERR);
        }
        LOG(LogLevel::INFO) << "create socket success";
    }

    void BindSocketOrDie(u_int16_t port) override
    {
        InetAddr local(port);
        if (bind(_sockfd, local.Addr(), local.Length()) != 0)
        {
            LOG(LogLevel::FATAL) << "bind socket error";
            exit(BIND_ERR);
        }
        LOG(LogLevel::INFO) << "bind socket success";
    }

    void ListenSocketOrDie(int backlog) override
    {
        if (listen(_sockfd, backlog) != 0)
        {
            LOG(LogLevel::FATAL) << "listen socket error";
            exit(LISTEN_ERR);
        }
        LOG(LogLevel::INFO) << "listen socket success";
    }

    // clientaddr输出型参数，存储客户端地址
    int Accept(InetAddr *clientaddr) override
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int fd = accept(_sockfd, (struct sockaddr *)&peer, &len);
        if (fd < 0)
        {
            LOG(LogLevel::WARNING) << "accept socket error";
            return -1;
        }
        LOG(LogLevel::INFO) << "accept socket success";
        clientaddr->Init(peer);
        return fd;
        // return std::make_shared<TcpSocket>(fd);
    }

    int Sockfd() override
    {
        return _sockfd;
    }

    void Close() override
    {
        if (_sockfd >= 0)
            close(_sockfd);
    }

    ssize_t Recv(std::string *out) override
    {
        // 只读一次
        char buffer[1024];
        ssize_t n = recv(_sockfd, buffer, sizeof(buffer) - 1, 0);
        if (n > 0)
        {
            buffer[n] = 0;
            *out += buffer;
        }
        return n;
    }

    ssize_t Send(const std::string &in) override
    {
        return send(_sockfd, in.c_str(), in.size(), 0);
    }

    bool Connect(InetAddr &peer) override
    {
        int n = connect(_sockfd, peer.Addr(), peer.Length());
        if (n >= 0)
            return true;

        return false;
    }

    ~TcpSocket()
    {
    }

private:
    int _sockfd;
};