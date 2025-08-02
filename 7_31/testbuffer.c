#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
    //C语言接口
    const char *s1="hello,printf\n";
    printf(s1);

    const char* s2="hello,fprintf\n";
    fprintf(stdout,s2);

    const char* s3="hello,fwrite\n";
    fwrite(s3,strlen(s3),1,stdout);

    //系统接口
    const char* s4="hello,write\n";
    write(1,s4,strlen(s4));

    fork();












    //close(1);
    //int fd=open("log.txt",O_CREAT|O_WRONLY |O_TRUNC,0666);
    //printf("hello world!,fd:%d\n",fd);
    //fflush(stdout); //语言缓冲区内容交给内核缓冲区
    //close(fd);   //将输出缓冲区关掉了，所以无法刷新出去

    



    //printf("1");
    //printf("2");
    //printf("3\n");
    //printf("4");
    //printf("5");
    //printf("6");
    //sleep(2);
    return 0;
}
