#pragma once

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
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
static int gbacklog=16;

//抽象基类，定义标准接口
class Socket
{
public:
    virtual ~Socket() {}   //虚析构函数，确保派生类函数正确释放资源

    //纯虚函数，定义socket操作接口
    virtual void CreateSocketOrDie() = 0;   //=0表示纯虚函数
    virtual void BindSocketOrDie(u_int16_t port) = 0;   //virtual表示这个函数可以在派生类中被重写
    virtual void ListenSocketOrDie(int backlog) = 0;
    virtual int Accept() = 0;

public:
    //模板方法，组合了socket的完整建立过程
    void BuildListenSocketMethod(int _port)
    {
        CreateSocketOrDie();
        BindSocketOrDie(_port);
        ListenSocketOrDie(gbacklog);
    }
};


//具体实现类（继承自socket类）
class TcpSocket : public Socket
{
public:
    TcpSocket():_sockfd(gsockfd)
    {
    }

    //必须实现所有虚函数
    void CreateSocketOrDie() override   //override表示正在重写虚函数
    {
        _sockfd=socket(AF_INET,SOCK_STREAM,0);
        if(_sockfd!=0)
        {
            LOG(LogLevel::FATAL)<<"create socket error";
            exit(CREATE_ERR);
        }
    }

    void BindSocketOrDie(u_int16_t port) override
    {
        InetAddr local(port);
        if(bind(_sockfd,local.Addr(),local.Length())!=0)
        {
            LOG(LogLevel::FATAL)<<"bind socket error";
            exit(BIND_ERR);
        }
    }

    void ListenSocketOrDie(int backlog) override
    {
        if(listen(_sockfd,backlog)!=0)
        {
            LOG(LogLevel::FATAL)<<"listen socket error";
            exit(LISTEN_ERR);
        }
    }

    int Accept() override
    {

    }

    ~TcpSocket()
    {}

private:
    int _sockfd;
};