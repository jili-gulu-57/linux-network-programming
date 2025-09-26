#include<stdio.h>
#include<signal.h>
#include<unistd.h>

int flag=0;

void handler(int signo)
{
    flag=1;
    printf("flag:0->1\n");
}

int main()
{
    signal(2,handler);
    printf("进程启动：%d\n",getpid());
    while(!flag);
    printf("进程正常结束\n");

    return 0;
}