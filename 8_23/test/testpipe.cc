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
        int cnt=0;
        std::string str="hello 父进程，我是子进程";
        std::string self=std::to_string(getpid());
        
        while(true)
        {
            char c='A'+cnt;
            write(pipefd[1],&c,1);
            cnt++;
            // std::cout<<"count:"<<cnt++<<std::endl;


            // std::string message=str+","+self+",";
            // message+=std::to_string(cnt++);
            // write(pipefd[1],message.c_str(),message.size());
            // sleep(1);
        }    
        close(pipefd[1]);
        std::cout<<"写端关闭"<<std::endl;
        sleep(5);
        exit(0);   
    }
    else
    {
        //父进程
        close(pipefd[1]);

        while(true)
        {
            char c;
            ssize_t n=read(pipefd[0],&c,1);
            if(n>0)
            {
                std::cout<<"读到数据："<< c <<std::endl;
            }
            else if(n==0)
            {
                std::cout<<"return val:"<< n <<std::endl;
                sleep(1);
            }
            else
            {
                std::cout<<"read error"<<std::endl;
                sleep(1);
            }
            break;
            //  sleep(1);
            // char inbuffer[1024]={0};
            // ssize_t n=read(pipefd[0],inbuffer,sizeof(inbuffer)-1);//为\0"预留位置
            // if(n>0)
            // {
            //     //读取成功
            //     inbuffer[n]=0;
            //     std::cout<<"client->father#"<<inbuffer<<std::endl;
            // }
        }
        close(pipefd[0]);
        std::cout<<"读端关闭"<<std::endl;

        int status=0;
       /*
        | 31-16位 | 15-8位 | 7位 | 6-0位 |
        |---------|--------|-----|-------|
        |  未使用  | 退出码  | 核心转储标志 | 信号编号 |
        */
        pid_t rid=waitpid(id,&status,0);    //表示不关注子进程退出状态，进行阻塞式等待
        if(rid>0)
        {
            int exitcode=(status>>8)&0xFF; //右移8位后，取低八位，获取状态退出码（0xFF是8个1）
            int exitsig=status&0x7F;   //取低七位，获取信号编号（0x7F是7个1）
            std::cout<<"wait success,rid:"<<rid<<",exit code:"<<exitcode<<",exitsig:"<<exitsig<<std::endl;
        }
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