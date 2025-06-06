#include<stdio.h>
#include<sys/types.h>
#include <unistd.h>
int main()
{
    pid_t id=fork();
    if(id==0)
    {
        //child
        while(1)
        {
            printf("我是一个子进程，pid：%d,ppid:%d\n",getpid(),getppid());
            sleep(2);
        }
    }
    else
    {
        //father
        int count=5;
        while(count--)
        {
            printf("我是一个父进程,pid:%d,ppid:%d\n",getpid(),getppid());
            sleep(2);
        }
    }
    return 0;
}
