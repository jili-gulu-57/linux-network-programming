#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>

int main()
{
    pid_t id=fork();
    if(id==0)
    {
        //子进程
        int cnt=10;
        while(cnt--)
        {
            printf("子进程运行中：%d\n",cnt);
            sleep(1);
        }
        exit(0);

    }
    //NULL表示不关心子进程的退出状态
    //0是默认选项，表示阻塞等待，直到目标进程退出
    //父进程需轮询等待
    while(1)
    {
        pid_t rid=waitpid(id,NULL,WNOHANG);
        //子进程退出，等待成功
        if(rid==id)
        {
            //等待成功
            printf("wait child success!\n");
            break;
        }
        //子进程仍在运行
        else if(rid==0)
        {
            printf("child not exit\n");
            sleep(2);
        }
        //rid<0 等待失败
        else 
        {
            printf("wait fail\n");
            break;
        }
    }
    return 0;
}
