#pragma once

#include <iostream>
#include <unistd.h>
#include <fcntl.h>

// ET模式下，必须将文件描述符设置成非阻塞，阻塞状态下进程会被卡住，无法执行后续代码

void SetNonBlock(int fd)
{
    // 1.先获取原来的状态标志（直接设置可能将其他位掩码覆盖）
    int fl = fcntl(fd, F_GETFL);
    // 获取失败
    if (fl < 0)
    {
        return;
    }
    // 获取成功
    // 2.设置非阻塞标志位并修改到文件描述符的状态标志
    fcntl(fd, F_SETFL, fl | O_NONBLOCK);
}