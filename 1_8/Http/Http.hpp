#pragma once

#include <iostream>
#include <unordered_map>
#include "TcpServer.hpp"
#include "Socket.hpp"
#include "Logger.hpp"

static const std::string linesep = "\r\n";
static const std::string innersep1 = " ";
static const std::string innersep2 = ": ";

class HttpRequset
{
private:
    std::string ReadOneLine(std::string &reqstr)
    {
        // 找到分隔符位置
        auto pos = reqstr.find(linesep);

        if (pos == std::string::npos)
            return std::string(); // 找不到则返回空串

        // 以分隔符位置截取字符串
        auto line = reqstr.substr(0, pos);

        // 提取完后从字符串中删除这段内容，以便下次提取
        reqstr.erase(0, pos + linesep.size());

        // 返回提取的字符串
        return line;
    }

    void ParseReqLine(std::string &reqline)
    {
        // GET / HTTP/1.1 -->      std::string _method;std::string _uri;std::string _httpversion;
        std::stringstream ss(reqline);
        ss >> _method >> _uri >> _httpversion;
    }

    void BuildKV(std::string &line, std::string *k, std::string *v)
    {
        // Connection:keep-alive
        auto pos = line.find(innersep2);
        if (pos == std::string::npos)
        {
            *k = *v = std::string();
            return;
        }
        *k = line.substr(0, pos);
        *v=line.substr(pos+innersep2.size());
    }

public:
    HttpRequset()
    {
    }

    void Serialize()
    {
        // 不做
    }

    bool Deserialize(std::string &reqstr)
    {
        std::string reqline = ReadOneLine(reqstr);
        if (reqline.empty())
            return false;

        LOG(LogLevel::DEBUG) << reqline;
        ParseReqLine(reqline);

        LOG(LogLevel::DEBUG) << "method: " << _method;
        LOG(LogLevel::DEBUG) << "uri: " << _uri;
        LOG(LogLevel::DEBUG) << "httpversion: " << _httpversion;

        return false;
    }

    ~HttpRequset()
    {
    }

private:
    std::string _method;
    std::string _uri;
    std::string _httpversion;
    std::unordered_map<std::string, std::string> _req_headers;
    std::string _blank_line;
    std::string _req_body;
};