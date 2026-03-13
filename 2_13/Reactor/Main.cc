#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include "Socket.hpp"
#include "Reactor.hpp"
#include "Connection.hpp"
#include "Listener.hpp"
#include "Calculator.hpp"
#include "Parser.hpp"
#include "Protocol.hpp"

void Usage(std::string proc)
{
    std::cout << "Usage: " << proc << " + port" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(0);
    }

    // 日志服务
    EnableConsoleLogStrategy();

    // 业务对象
    std::unique_ptr<Calculator> cal = std::make_unique<Calculator>();

    // 协议和解析协议对象
    std::unique_ptr<Parser> parser_protocol = std::make_unique<Parser>(
        [&cal](Request &req) -> ResPonse
        {
            return cal->Exec(req);
        });

    // 网络服务
    uint16_t serverport = std::stoi(argv[1]);

    // 连接监听管理器
    std::shared_ptr<Connection> listener = std::make_shared<Listener>(serverport);
    listener->SetCallback([&parser_protocol](std::string &inbuffer) -> std::string
                          { return parser_protocol->Parse(inbuffer); });

    // 构建一个Reactor容器
    std::unique_ptr<Reactor> R = std::make_unique<Reactor>();

    // 把连接管理器添加到Reactor
    R->AddConnection(listener);

    // 启动Reactor
    R->Dispatcher();

    return 0;
}