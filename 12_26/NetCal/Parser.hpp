#pragma once

#include <iostream>
#include <string>
#include "Protocol.hpp"
#include "Logger.hpp"
#include <functional>

using handler_t = std::function<ResPonse(Request &req)>;

class Parser
{
public:
    Parser(handler_t handler)
        : _handler(handler)
    {
    }

    // 根据协议负责网络通信中的粘包拆包，序列化和反序列化问题
    std::string Parse(std::string &inbuffer)
    {
        std::string send_str;
        while (true)
        {
            // 1.拆包
            std::string jsonstr;
            // 解析报文
            int n = Protocol::Unpack(inbuffer, &jsonstr);
            if (n < 0)
            {
                exit(0);
            }
            else if (n == 0)
            {
                break;
            }

            else
            {
                //打印jsonstr
                LOG(LogLevel::DEBUG)<<"\r\n"<<jsonstr;
                // 2.反序列化（拆包成功）
                Request req;
                if (!req.Deserialize(jsonstr))
                {
                    // 反序列化失败，返回空串
                    return std::string();
                }

                // 3.对数据进行处理（反序列化成功）
                ResPonse resp = _handler(req);

                // 4.对结果（resp）进行序列化
                std::string resp_json;
                if (!resp.Serialize(&resp_json))
                {
                    return std::string();
                }

                // 5.打包
                send_str += Protocol::Package(resp_json);
            }
        }
        return send_str;
    }

    ~Parser()
    {
    }

private:
    handler_t _handler;
};