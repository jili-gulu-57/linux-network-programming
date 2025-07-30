#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    //pid_t id=fork();
    //if(id==0)
    //{
    //    //子进程
    //    sleep(1);
    //    close(1);
    //    while(1)
    //    {
    //        sleep(2);
    //    }
    //}
    ////父进程
    //while(1)
    //{
    //    printf("hello,world!\n");
    //}





    //打开目标文件
    int fd=open("log.txt",O_CREAT|O_WRONLY|O_TRUNC,0666);
    if(fd<0)
    {
        perror("fd");
        return 1;
    }
    //输出重定向
    dup2(fd,1);
    //printf("hello,world!\n");
    //exec不影响重定向
    execl("/usr/bin/")




























    //close(0);
    ////int fd=open("log.txt",O_CREAT|O_WRONLY|O_TRUNC,0666);
    ////int fd=open("log.txt",O_CREAT|O_WRONLY|O_APPEND,0666);
    //int fd=open("log.txt",O_RDONLY);
    //if(fd<0)
    //{
    //    perror("fd");
    //    return 1;
    //}
    //int a,b,c;
    //scanf("%d%d%d",&a,&b,&c);
    //printf("%d,%d,%d",a,b,c);
    //printf("hello world!fd:%d\n",fd);//默认打印到标准输出stdout ->1
    //close(fd);
}
