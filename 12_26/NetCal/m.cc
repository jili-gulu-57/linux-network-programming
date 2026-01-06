// test_socket_simple.cpp
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

int main() {
    std::cout << "=== 测试socket创建 ===" << std::endl;
    
    // 测试1: 最简单的socket创建
    std::cout << "\n1. 测试socket()函数..." << std::endl;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockfd < 0) {
        std::cerr << " socket() 失败!" << std::endl;
        std::cerr << "错误码: " << errno << std::endl;
        std::cerr << "错误信息: " << strerror(errno) << std::endl;
        
        // 常见错误码分析
        switch(errno) {
            case EACCES: 
                std::cerr << "原因: 权限不足" << std::endl;
                std::cerr << "解决: 尝试使用sudo或1024以上端口" << std::endl;
                break;
            case EAFNOSUPPORT: 
                std::cerr << "原因: 地址族不支持" << std::endl;
                break;
            case EMFILE: 
                std::cerr << "原因: 进程文件描述符用尽" << std::endl;
                std::cerr << "解决: ulimit -n 查看和增加限制" << std::endl;
                break;
            case ENFILE: 
                std::cerr << "原因: 系统文件描述符用尽" << std::endl;
                break;
            case ENOMEM: 
                std::cerr << "原因: 内存不足" << std::endl;
                break;
            case EPROTONOSUPPORT: 
                std::cerr << "原因: 协议不支持" << std::endl;
                break;
            default:
                std::cerr << "未知错误" << std::endl;
        }
        return 1;
    }
    
    std::cout << " socket() 成功! fd = " << sockfd << std::endl;
    
    // 测试2: 获取socket选项
    std::cout << "\n2. 测试socket选项..." << std::endl;
    int optval;
    socklen_t optlen = sizeof(optval);
    
    if (getsockopt(sockfd, SOL_SOCKET, SO_TYPE, &optval, &optlen) == 0) {
        std::cout << "socket类型: " 
                  << (optval == SOCK_STREAM ? "SOCK_STREAM(TCP)" : "其他") 
                  << std::endl;
    }
    
    // 测试3: 关闭socket
    std::cout << "\n3. 关闭socket..." << std::endl;
    if (close(sockfd) == 0) {
        std::cout << " socket关闭成功" << std::endl;
    } else {
        std::cerr << " socket关闭失败: " << strerror(errno) << std::endl;
    }
    
    std::cout << "\n=== 测试完成 ===" << std::endl;
    return 0;
}