#pragma once

#include "common.hpp"

const int defaultfd = -1;

class NamedPipe
{
public:
    NamedPipe(std::string name)
        : _name(name),
          _fd(defaultfd)
    {
    }

    ~NamedPipe()
    {
    }

    // 创建管道
    bool Creat()
    {
        // 1.创建管道文件
        int n = mkfifo(fifoname.c_str(), mode);
        if (n == 0)
        {
            std::cout << "mkfifo success" << std::endl;
        }
        else
        {
            // std::cout<<"mkfifo failed"<<std::endl;
            perror("mkfifo"); // 将错误码转化成信息
            return false;
        }
        return true;
    }

    // 打开文件 --> 写入
    bool OpenForWrite()
    {
        _fd = open(fifoname.c_str(), O_WRONLY);
        if (_fd < 0)
        {
            perror("open");
            return false;
        }
        return true;
    }

    // 打开文件 --> 读取
    bool OpenForRead()
    {
        _fd = open(fifoname.c_str(), O_RDONLY);
        if (_fd < 0)
        {
            perror("open");
            return false;
        }
        // 命名管道只有在另一端也启动的情况下，才会打开管道文件，否则就会阻塞
        std::cout << "open success" << std::endl;
        return true;
    }

    // 输出型参数 *
    bool Read(std::string *out)
    {
        char buffer[SIZE] =  {0};
        buffer[0] = 0;
        ssize_t num = read(_fd, buffer, sizeof(buffer) - 1);
        if (num > 0)
        {
            buffer[num] = 0;
            *out = buffer;
            return true;
        }
        else if (num == 0)
        {
            std::cout << "client quit!" << std::endl;
            return false;
        }
        else
        {
            return false;
        }
    }

    // 输入型参数 const &
    void Write(const std::string &in)
    {
        write(_fd, in.c_str(), in.size());
    }

    // 关闭管道
    void Close()
    {
        if (_fd == defaultfd)
            return;
        else
            close(_fd);
    }

    // 移除
    void Remove()
    {
        int m = unlink(fifoname.c_str());
        (void)m;
    }

private:
    std::string _name; // 命名管道的名字
    int _fd;
};
