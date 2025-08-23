#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<unistd.h>
#include<string>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
    //父子通信：子写，父读

    //1.父进程创建管道文件
    int pipefd[2]={0};
    int n=pipe(pipefd);

    (void)n;//防止编译器报警

    //2.fork函数创建子进程
    pid_t id=fork();
    if(id<0)
    {
        perror("fork fail!");
        exit(1);
    }

    //3.父子进程关闭不必要的端
    else if(id==0)
    {
        //子进程
        close(pipefd[0]);
        int cnt=1;
        std::string str="hello 父进程，我是子进程";
        std::string self=std::to_string(getpid());
        std::string message=str+","+self+",";
        while(true)
        {
            message+=std::to_string(cnt++);
            write(pipefd[1],message.c_str(),message.size());
            sleep(1);
        }
    }
    else
    {
        //父进程
        close(pipefd[1]);

        while(true)
        {
            sleep(3);
            char inbuffer[1024]={0};
            ssize_t n=read(pipefd[0],inbuffer,sizeof(inbuffer)-1);//为\0"预留位置
            if(n>0)
            {
                //读取成功
                inbuffer[n]=0;
                std::cout<<"client->father#"<<inbuffer<<std::endl;
            }
        }

        pid_t rid=waitpid(id,nullptr,0);
        //表示不关注子进程退出状态，进行阻塞式等待
        (void)rid;
    }
    //4.进行通信

    //5.关闭管道

    // int pipefd[2]={0};
    // int n=pipe(pipefd);
    // if(n==0)
    // {
    //     // std::cout<<"creat pipe success!"<<std::endl;
    //     printf("creat pipe success,pipefd[0]:%d,pipefd[1]:%d\n",pipefd[0],pipefd[1]);
    // }
    return 0;
}