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

static const std::string sep = "\r\n";
class Protocol
{
public:
    // 打包
    static std::string Package(std::string &jsonstr)
    {
        if (jsonstr.empty())
            return jsonstr;

        std::string json_length = std::to_string(jsonstr.size());
        return json_length + sep + jsonstr + sep;
    }

    // 报头有效性检查
    bool DigitalIsSafe(std::string &len)
    {
        for (int i = 0; i < len.size(); i++)
        {
            if (!(len[i] >= '0' && len[i] <= '9'))
                return false;
        }
        return true;
    }

    // 解包
    static bool Unpack(std::string *origin_str, std::string *package)
    {
        // 字符串为空
        if (!origin_str)
            return false;

        // len\r\njsonstr\r\n
        // len\r\n
        // len\r\njsonstr\r\nlen\r\njsonstr\r\n
        // len\r\njsonstr
        // len\r\njsonstr\r\nlen\r\n
        // len\r\njsonstr\r\nlen\r\njsonstr
        // len
        auto pos = origin_str->find(sep);
        if (pos == std::string::npos)
        {
            // 没找到分隔符位置，绝对不是有效报文
            return false;
        }

        std::string len = origin_str->substr(0, pos);
    }
};