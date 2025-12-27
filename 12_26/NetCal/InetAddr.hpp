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

#define Conv(addr) ((struct sockaddr *)&addr)

class InetAddr
{
private:
    // 网络序列转主机序列
    void Net2Host()
    {
        u_int16_t clientport = ntohs(_addr.sin_port);
        std::string clientip = inet_ntoa(_addr.sin_addr);
    }

    // 主机转网络序列
    void Host2Net()
    {
        memset(&_addr, 0, sizeof(_addr));
        _addr.sin_family = AF_INET;
        _addr.sin_port = htons(_port);
        _addr.sin_addr.s_addr = inet_addr(_ip.c_str());
        //_ip.c_str()：将C++ string转换为C风格字符串
        // inet_addr()：将点分十进制IP字符串转换为网络字节序的32位整数
    }

public:
    InetAddr(const struct sockaddr_in &addr)
        : _addr(addr) // 传参为网络序列
    {
        // 调用H->N
        Net2Host();
    }

    // 传参为主机序列
    InetAddr(u_int16_t &port, const std::string &ip = "0.0.0.0")
        : _port(port), _ip(ip)
    {
        Host2Net();
    }

    std::string Ip()
    {
        return _ip;
    }

    u_int16_t Port()
    {
        return _port;
    }

    struct sockaddr *Addr()
    {
        return Conv(_addr);
    }

    socklen_t Length()
    {
        return sizeof(_addr);
    }

    std::string ToString()
    {
        std::string addr=_ip + "-" + std::to_string(_port);
        return addr;
    }

    // 重载==运算符，判断两个客户端地址是否相同
    bool operator==(const InetAddr &addr)
    {
        return (_ip == addr._ip && _port == addr._port);
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