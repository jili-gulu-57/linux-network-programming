#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
//void fun()
//{
//    printf("fun begin\n");
//    _exit(2);
//    printf("fun end\n");
//}
int main()
{

    pid_t id=fork();
    if(id==0)
    {
        //child
        int count=3;
        while(count--)
        {
            sleep(1);
            printf("我是一个子进程，我的id：%d,父进程id：%d\n",getpid(),getppid());
        }
        exit(10);
    }
   
    //parent
    while(1)
    {
        int status=0;
        pid_t rid=waitpid(id,&status,WNOHANG);
        if(rid>0)
        {
            printf("wait success,rid:%d,exit code:%d,exit signal:%d\n ",rid,(status>>8)&0xFF,status&0x7F);
            break;
        }
        else if(rid==0)
        {
            printf("本轮等待结束，子进程没有退出\n");
            sleep(1);
        }
        else 
        {
            printf("等待失败\n");
        }
    }
    //int status=0;
    //pid_t rid=waitpid(id,&status,0);
    //printf("我是一个子进程，我的id：%d,父进程id：%d\n",getpid(),getppid());
    //if(rid>0)
    //{
    //     printf("wait success,rid:%d,exit code:%d,exit signal:%d\n ",rid,(status>>8)&0xFF,status&0x7F);
    //}
    //else 
    //{
    //    printf("wait fail:%d:%s\n",errno,strerror(errno));
    //}

    

    //fun();
    ////exit(0);
    //printf("main begin\n");
    //int i=0;
    //for(;i<150;i++)
    //{
    //    printf("%d:%s\n",i,strerror(i));
    //}

    //
    //FILE* fp=fopen("log.txt","r");
    //if(fp==NULL)
    //    return errno;
    //printf("hello world!\n");
    return 0;
}
