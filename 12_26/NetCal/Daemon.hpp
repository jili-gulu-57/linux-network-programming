#pragma once

#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

void Daemon()
{
    signal(SIGCHLD, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);

    //让进程不是组长进程
    if (fork() > 0)
    {
        // 父进程直接退出
        exit(0);
    }

    //更改守护进程的工作路径，建议是/
    chdir("/");

    //变成守护进程
    setsid();

    int fd = open("/dev/null", O_RDWR);
    if (fd > 0)
    {
        //将fd文件描述符拷贝到文件描述符表的0、1、2号位置
        dup2(fd, 0);
        dup2(fd, 1);
        dup2(fd, 2);
    }
}