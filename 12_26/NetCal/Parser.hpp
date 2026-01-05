#include <iostream>
#include <string>
#include "Protocol.hpp"

class Parser
{
public:
    Parser()
    {
    }

    std::string Parse(std::string &inbuffer)
    {
        std::string send_str;
        while (true)
        {
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
                Request req;
                // 反序列化
                if (!req.Deserialize(jsonstr))
                {
                    // 反序列化失败，返回空串
                    return std::string();
                }

                ResPonse resp = a(req);

                // 对resp进行序列化
                std::string resp_json;
                if (!resp.Serialize(&resp_json))
                {
                    return std::string();
                }

                // 打包
                send_str += Protocol::Package(resp_json);
            }
        }
        return send_str;
    }

    ~Parser()
    {
    }
};