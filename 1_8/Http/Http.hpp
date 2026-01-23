#pragma once

#include <iostream>
#include <unordered_map>
#include <fstream>
#include "TcpServer.hpp"
#include "Socket.hpp"
#include "Logger.hpp"

// http协议中的换行符
static const std::string linesep = "\r\n";
// 请求行中的分隔符
static const std::string innersep1 = " ";
// http头部键值对的分隔符
static const std::string innersep2 = ": ";

static const std::string webroot = "./wwwroot";
static const std::string defaulthome = "index.html";

static const std::string suffixsep = "."; // 后缀分割符

// 用于解析浏览器（客户端）发来的请求的类
class HttpRequset
{
private:
    // 读取一行http内容
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

    // 解析请求行
    void ParseReqLine(std::string &reqline)
    {
        // GET / HTTP/1.1 -->      std::string _method;std::string _uri;std::string _httpversion;
        std::stringstream ss(reqline);         // 使用字符串流方便分割
        ss >> _method >> _uri >> _httpversion; // 分别提取方法，URI，http版本
    }

    // 构造键值对
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

    // 反序列化http请求
    bool Deserialize(std::string &reqstr)
    {
        bool status = true;
        std::string reqline = ReadOneLine(reqstr, &status);
        if (!status)
            return false;

        LOG(LogLevel::DEBUG) << reqline;
        // 解析请求行
        ParseReqLine(reqline);

        // LOG(LogLevel::DEBUG) << "method: " << _method;
        // LOG(LogLevel::DEBUG) << "uri: " << _uri;
        // LOG(LogLevel::DEBUG) << "httpversion: " << _httpversion;

        // 解析头部
        while (true)
        {
            status = true;
            reqline = ReadOneLine(reqstr, &status);
            if (status && !reqline.empty())
            {
                std::string k, v;
                BuildKV(reqline, &k, &v);
                if (k.empty() || v.empty())
                    continue; // 解析失败这次循环停止

                _req_headers.insert(std::make_pair(k, v));
            }
            else if (status) // 空行，表示头部结束
            {
                _blank_line = linesep;
                break;
            }
            else
            {
                LOG(LogLevel::DEBUG) << "非法请求";
                break;
            }
            _req_body = reqstr;

            _path = webroot;
            _path += _uri;
            if (_uri == "/")
            {
                _path += defaulthome;
            }
            LOG(LogLevel::DEBUG) << "path:" << _path;
        }
        return true;
    }

    std::string Path()
    {
        return _path;
    }

    std::string Suffix()
    {
        // path: index.html
        if (_path.empty())
            return std::string();
        else
        {
            auto pos = _path.rfind(suffixsep); // 从后往前查找
            if (pos == std::string::npos)
                return std::string();
            return _path.substr(pos);
        }
    }

    ~HttpRequset()
    {
    }

private:
    std::string _method;                                       // http方法，GET，POST等
    std::string _uri;                                          // 请求uri，/index.html等
    std::string _httpversion;                                  // http版本
    std::unordered_map<std::string, std::string> _req_headers; // 请求头部键值对
    std::string _blank_line;                                   // 空行，分割头部和正文
    std::string _req_body;                                     // 请求正文
    std::string _path;                                         // 请求文件的物理路径（解析后的路径）
    std::vector<std::string> _cookie;
};

class HttpResponse
{
public:
    HttpResponse() : _httpversion("HTTP/1.1"), _blank_line(linesep)
    {
    }

    // 序列化http响应
    std::string Serialize()
    {
        // 构建响应行，如HTTP/1.1 200 OK\r\n
        std::string respstr = _httpversion + innersep1 + std::to_string(_code) +
                              innersep1 + _desc + linesep;

        // 判断响应正文是否为空
        if (!_resp_body.empty())
        {
            std::string len = std::to_string(_resp_body.size());
            SetHeader("Content-Length", len);
        }

        // 构建响应头部
        for (auto &elem : _resp_headers)
        {
            std::string line = elem.first + innersep2 + elem.second + linesep;
            respstr += line;
        }

        respstr += _blank_line; // 添加空行

        respstr += _resp_body; // 添加正文

        return respstr;
    }

    void Deserialize()
    {
        // 不做
    }

    // 读取文件内容
    bool ReadContent(const std::string &path)
    {
        // 以二进制方式读取
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open())
        {
            LOG(LogLevel::WARNING) << path << "资源不存在";
            return false;
        }

        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        _resp_body.resize(static_cast<size_t>(fileSize));

        file.read(&_resp_body[0], fileSize);

        file.close();
        return true;
    }

    std::string Code2Desc(int code)
    {
        switch (code)
        {
        case 200:
            return "OK";
        case 404:
            return "NOT FOUND";
        default:
            return "";
        }
    }

    // 设置状态码和状态码描述
    void SetCode(int code)
    {
        // 1xx-5xx的状态码才是合法的
        if (code >= 100 && code < 600)
        {
            _code = code;
            _desc = Code2Desc(_code);
        }
        else
        {
            LOG(LogLevel::WARNING) << "非法状态码：" << code;
        }
    }

    void SetHeader(const std::string &k, const std::string &v)
    {
        _resp_headers[k] = v;
    }

    void SetCookie(std::string key,std::string value)
    {

    }

    ~HttpResponse()
    {
    
    }

private:
    std::string _httpversion;                                   // http版本
    int _code;                                                  // 状态码，202，404等
    std::string _desc;                                          // 状态码描述，如OK，NOT，FOUND等
    std::unordered_map<std::string, std::string> _resp_headers; // 响应头部
    std::string _blank_line;                                    // 空白行
    std::string _resp_body;                                     // 响应正文
    std::vector<std::string> _cookie;
};

class Http
{
    std::string Suffix2Desc(const std::string &suffix)
    {
        if (suffix == "html")
            return "text/html";
        else if (suffix == ".jpg")
            return "image/jpeg";
        else
            return std::string();
    }

public:
    Http()
    {
    }

    // 处理http请求
    std::string HandlerRequset(std::string &requeststr)
    {
        std::string respstr;
        HttpRequset req;

        // 解析请求
        if (req.Deserialize(requeststr))
        {
            HttpResponse resp;
            // 读取请求的文件
            if (resp.ReadContent(req.Path()))
            {
                std::string suffix = req.Suffix();
                std::string mime_type_value = Suffix2Desc(suffix); // 将资源后缀转换为文件类型（Content-Type）
                resp.SetHeader("Content-Type", mime_type_value);
                // 设置响应状态
                resp.SetCode(200);
            }
            else
            {
                resp.SetCode(404);
            }
            // 序列化响应
            respstr = resp.Serialize();
        }
        return respstr;
    }

    ~Http()
    {
    }
};