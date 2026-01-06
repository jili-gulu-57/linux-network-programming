#pragma once

#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>

class Request
{
public:
    Request()
    {
        _x = _y = _oper = 0;
    }

    // 序列化对象
    bool Serialize(std::string *out)
    {
        Json::Value root;
        root["x"] = _x;
        root["y"] = _y;
        root["oper"] = _oper;

        Json::StyledWriter writer;
        *out = writer.write(root);
        if (out->empty())
        {
            return false;
        }
        return true;
    }

    // 反序列化对象
    bool Deserialize(std::string &in)
    {
        Json::Reader reader;
        Json::Value root;
        bool ret = reader.parse(in, root);
        if (!ret)
            return false;

        _x = root["x"].asInt();
        _y = root["y"].asInt();
        _oper = root["oper"].asInt();

        return true;
    }

    int X()
    {
        return _x;
    }

    int Y()
    {
        return _y;
    }

    char Oper()
    {
        return _oper;
    }

    ~Request()
    {
    }

public:
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
        _result = _code = 0;
    }

    // 序列化对象
    bool Serialize(std::string *out)
    {
        Json::Value root;
        root["result"] = _result;
        root["code"] = _code;

        Json::StyledWriter writer;
        *out = writer.write(root);
        if (out->empty())
        {
            return false;
        }
        return true;
    }

    // 反序列化对象
    bool Deserialize(std::string &in)
    {
        Json::Reader reader;
        Json::Value root;
        bool ret = reader.parse(in, root);
        if (!ret)
            return false;

        _result = root["result"].asInt();
        _code = root["code"].asInt();

        return true;
    }

    void Result(int r)
    {
        _result = r;
    }

    void Code(int c)
    {
        _code = c;
    }

    void Print()
    {
        std::cout <<"结果为："<< _result << std::endl;
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
    static bool DigitalIsSafe(std::string len)
    {
        for (int i = 0; i < len.size(); i++)
        {
            if (!(len[i] >= '0' && len[i] <= '9'))
                return false;
        }
        return true;
    }

    // 解包
    static int Unpack(std::string &origin_str, std::string *package)
    {
        // 字符串为空
        if (origin_str.empty())
            return 0;

        // len\r\njsonstr\r\n
        // len\r\n
        // len\r\njsonstr\r\nlen\r\njsonstr\r\n
        // len\r\njsonstr
        // len\r\njsonstr\r\nlen\r\n
        // len\r\njsonstr\r\nlen\r\njsonstr
        // len
        auto pos = origin_str.find(sep);
        if (pos == std::string::npos)
        {
            // 没找到分隔符位置，绝对不是有效报文
            return 0;
        }

        std::string len = origin_str.substr(0, pos); // 报头长度
        // 报头有效性检查
        if (!DigitalIsSafe(len))
            return -1;

        int digit_len = std::stoi(len);                            // 有效数据长度
        int target_len = len.size() + digit_len + 2 * sizeof(sep); // 报文整体长度

        // 有效报文长度小于应有长度
        if (origin_str.size() < digit_len)
            return 0;

        // 处理有效报文
        // 提取有效报文
        *package = origin_str.substr(pos + sep.size(), digit_len);

        // 原字符串移除已提取的报文
        origin_str.erase(0, target_len);
        return target_len;
    }
};