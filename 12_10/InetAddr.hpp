#pragma once

// 用于描述管理client socket的信息的类
// 先描述后组织

#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

class InetAddr
{
public:
    InetAddr(const struct sockaddr_in &addr) : _addr(addr)  //传参为网络序列
    {
        //调用N->H
        Net2Host();
    }

    //传参为主机序列
    InetAddr(const std::string&ip,u_int16_t&port):_ip(ip),_port(port)
    {

    }

    // 网络序列转主机序列
    void Net2Host()
    {
        u_int16_t clientport = ntohs(_addr.sin_port);
        std::string clientip = inet_ntoa(_addr.sin_addr);
    }

    //主机转网络序列
    void Host2Net()
    {
        memset(&_addr,0,sizeof(_addr));
        _addr.sin_family=AF_INET;
        _addr.sin_port=htons(_port);
        _addr.sin_addr.s_addr=inet_addr(_ip.c_str());
    }

    ~InetAddr()
    {
    }

private:
    // 网络风格地址
    struct sockaddr_in _addr;

    // 主机风格地址
    std::string _ip;
    u_int16_t _port;
};