#include "Socket.hpp"
#include "TcpServer.hpp"
#include "Protocol.hpp"
#include "Calculator.hpp"
#include "Parser.hpp"
#include <memory>

// ┌─────────────────────────────────────────────────────────┐
// │                   客户端请求                             │
// │  {"op":"add", "a":10, "b":20}                           │
// └─────────────────────────┬───────────────────────────────┘
//                           │ TCP连接
//                           ▼
// ┌─────────────────────────────────────────────────────────┐
// │                   TcpServer                             │
// │  • 监听端口                                              │
// │  • 接受连接                                              │
// │  • fork子进程处理                                        │
// └─────────────────────────┬───────────────────────────────┘
//                           │ 调用回调函数
//                           ▼
// ┌─────────────────────────────────────────────────────────┐
// │                  Lambda回调                             |
// │  [&par](std::string &inbuffer) {                        │
// │      return par->Parse(inbuffer);                       │
// │  }                                                      │
// └─────────────────────────┬───────────────────────────────┘
//                           │ 调用Parser::Parse
//                           ▼
// ┌─────────────────────────────────────────────────────────┐
// │                      Parser                             │
// │  1. Unpack() 拆包                                       |
// │  2. Deserialize() 反序列化 → Request对象                 │
// │  3. 调用业务回调                                         │
// │  4. Serialize() 序列化 → JSON字符串                      │
// │  5. Package() 打包                                      │
// └─────────────────────────┬───────────────────────────────┘
//                           │ 调用Calculator回调
//                           ▼
// ┌─────────────────────────────────────────────────────────┐
// │                   Calculator                            │
// │  Response Exec(Request &req) {                          │
// │      if (req.op == "add")                               │
// │          return req.a + req.b;                          │
// │       // .. 其他运算                                     │
// │  }                                                      │
// └─────────────────────────┬───────────────────────────────┘
//                           │ 返回Response对象
//                           ▼
// ┌─────────────────────────────────────────────────────────┐
// │                  响应流程（逆序）                        │
// │  Response → Parser → Lambda → TcpServer → 客户端        │
// └─────────────────────────────────────────────────────────┘

void Usage(std::string proc)
{
    std::cout << "Usage:" << proc << "localport" << std::endl;
}

int main(int argc, char *argv[])
{
    // 参数检查
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(0);
    }

    EnableConsoleLogStrategy();

    // 创建Calculator（业务处理）
    std::unique_ptr<Calculator> cal = std::make_unique<Calculator>();

    // 创建Parser（协议解析）
    std::unique_ptr<Parser> par = std::make_unique<Parser>([&cal](Request &req) -> ResPonse // 返回类型：Response
                                                           { return cal->Exec(req); });


    // 创建TcpServer（网络通信）
    u_int16_t serverport = std::stoi(argv[1]);
    std::unique_ptr<TcpServer> tsock = std::make_unique<TcpServer>(serverport, [&par](std::string &inbuffer) -> std::string
                                                                   { return par->Parse(inbuffer); });
    
    tsock->Run();

    return 0;
}