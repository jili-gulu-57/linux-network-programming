#pragma once

#include <iostream>
#include <unordered_map>
#include <fstream>
#include "TcpServer.hpp"
#include "Socket.hpp"
#include "Logger.hpp"

//http协议中的换行符
static const std::string linesep = "\r\n";
//请求行中的分隔符
static const std::string innersep1 = " ";
//http头部键值对的分隔符
static const std::string innersep2 = ": ";


//用于解析浏览器（客户端）发来的请求的类
class HttpRequset
{
private:
    std::string ReadOneLine(std::string &reqstr, bool *status)
    {
        // 找到分隔符位置
        auto pos = reqstr.find(linesep);

        if (pos == std::string::npos)
        {
            *status = false;
            return std::string(); // 找不到则返回空串
        }
        *status = true;
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
        *v = line.substr(pos + innersep2.size());
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
        bool status = true;
        std::string reqline = ReadOneLine(reqstr, &status);
        if (!status)
            return false;

        LOG(LogLevel::DEBUG) << reqline;
        ParseReqLine(reqline);

        LOG(LogLevel::DEBUG) << "method: " << _method;
        LOG(LogLevel::DEBUG) << "uri: " << _uri;
        LOG(LogLevel::DEBUG) << "httpversion: " << _httpversion;

        while (true)
        {
            status = true;
            reqline = ReadOneLine(reqstr, &status);
            if (status && !reqline.empty())
            {
                std::string k, v;
                BuildKV(reqline, &k, &v);
            }
            else if (status)
            {
                _blank_line = linesep;
                break;
            }
            else
            {
                LOG(LogLevel::DEBUG) << "非法请求";
                break;
            }
        }

        return true;
    }

    std::string Path()
    {
        return _path;
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
    std::string _resp_body;

    std::string _path;
};

class HttpResponse
{
public:
    HttpResponse() : _httpversion("HTTP/1.1"), _blank_line("\r\n")
    {
    }

    std::string Serialize()
    {
        std::string respstr = _httpversion + innersep1 + std::to_string(_code) +
                              innersep1 + _desc + linesep;

        for (auto &elem : _resp_headers)
        {
            std::string line = elem.first + innersep2 + elem.second + linesep;
            respstr += line;
        }

        respstr += _blank_line;
        respstr += _resp_body;

        return respstr;
    }

    void Deserialize()
    {
        // 不做
    }

    void ReadContent(const std::string &path)
    {
        // 以二进制方式读取
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error("无法打开：" + path);
        }

        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        _resp_body.resize(static_cast<size_t>(fileSize));

        file.read(&_resp_body[0], fileSize);

        if (!file)
        {
            throw std::runtime_error("读取文件失败：" + path);
        }
        file.close();
    }

    // 设置状态码和状态码描述
    void SetCode(int code, const std::string &desc)
    {
        _code = code;
        _desc = desc;
    }

    ~HttpResponse()
    {
    }

private:
    std::string _httpversion;
    int _code;
    std::string _desc;
    std::unordered_map<std::string, std::string> _resp_headers;
    std::string _blank_line;
    std::string _resp_body;
};

class Http
{
public:
    Http()
    {
    }

    std::string HandlerRequset(std::string &requeststr)
    {
        std::string respstr;
        HttpRequset req;
        if (req.Deserialize(requeststr))
        {
            HttpResponse resp;
            resp.ReadContent(req.Path());
            resp.SetCode(200, "OK");
            respstr = resp.Serialize();
        }
    }

    ~Http()
    {
    }
};