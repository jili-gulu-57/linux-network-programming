#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
int main()
{
    while(1)
    {
        printf("我是一个进程，我的id是：%d,我的父进程是：%d\n",getpid(),getppid());
        sleep(1);
    }
    return 0;
}

