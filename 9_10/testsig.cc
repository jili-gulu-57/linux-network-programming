#include<iostream>
#include<sys/types.h>
#include<signal.h>
#include<unistd.h>

void handler(int signo)
{
    printf("我是进程：%d,捕获到了信号：%d\n",getpid(),signo);
}

int main()
{
    // signal(2,SIG_IGN);  //对2号信号进行忽视
    // signal(2,SIG_DFL);  //对2号进行默认操作

    signal(SIGINT,handler); //对2号信号自定义处理方法
    while(true)
    {
        printf("我是一个进程：%d\n",getpid());
        sleep(1);
    }

    return 0;
}