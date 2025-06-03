#include <stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main()
{
    pid_t id=fork();
    if(id==0)
    {
        //子进程
        int count=5;
        while(count)
        {
            printf("我是一个子进程，我正在运行\n");
            count--;
            sleep(1);
        }
    }
    else{

        //父进程
        while(1)
        {
             printf("我是一个父进程，我正在运行\n");
            sleep(1);
        }
    }
    //printf("我是一个进程，pid：%d\n",getpid());
    //int x;
    //scanf("%d",&x);
    //while(1)
    //{
    //    //printf("hello wq!\n");
    //}
    return 0;
}
