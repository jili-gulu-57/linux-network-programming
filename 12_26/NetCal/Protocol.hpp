#pragma once

#include <iostream>
#include <string>

class Request
{
public:
    Request()
    {
    }

    // 序列化对象
    std::string Serialize()
    {
    }

    // 反序列化对象
    void Deserialize()
    {
    }

    ~Request()
    {
    }

private:
    //  x oper y
    int _x;
    int _y;
    char _oper;
};

class ResPonse
{
public:
    ResPonse()
    {
    }

    // 序列化对象
    std::string Serialize()
    {
    }

    // 反序列化对象
    void Deserialize()
    {
    }

    ~ResPonse()
    {
    }

private:
    int _result; // 计算结果
    int _code;   // 结果可靠性（0：可靠 1、2、3……：出错类型）
};