#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define VERSION1 (1<<0)  //1
#define VERSION2 (1<<1)  //2
#define VERSION3 (1<<2)  //4
#define VERSION4 (1<<3)  //8
#define VERSION5 (1<<4)  //16

int main()
{
    int fd=open("log.txt",O_WRONLY|O_CREAT|O_APPEND,0666);
    //失败时文件描述符返回-1
    if(fd<0)
    {
        perror("open fail");
        return 1;
    }
    const char *str="123hhh\n";
    write(fd,str,strlen(str));
    //关闭文件
    close(fd);
    return 0;
}

//void ShowVersion(int flags)
//{
//    if(flags & VERSION1)
//        printf("VERSION1\n");
//    if(flags & VERSION2)
//        printf("VERSION2\n");
//    if(flags & VERSION3)
//        printf("VERSION3\n");
//    if(flags & VERSION4)
//        printf("VERSION4\n");
//    if(flags & VERSION5)
//        printf("VERSION5\n");
//}
//
//int main()
//{
//    ShowVersion(VERSION1);
//    printf("VERSION1\n");
//    printf("---------------------------\n");
//
//    ShowVersion(VERSION3);
//    printf("VERSION3\n");
//    printf("---------------------------\n");
//
//    ShowVersion(VERSION1|VERSION5);
//    printf("VERSION1和VERSION5\n");
//    printf("---------------------------\n");
//
//    ShowVersion(VERSION2|VERSION4);
//    printf("VERSION2和VERSION$\n");
//    printf("---------------------------\n");
//}



















//int main()
//{
//    const char *s1="hello printf\n";
//    printf(s1);
//
//    const char *s2="hello fprintf\n";
//    fprintf(stdout,s2);
//
//    const char *s3="hello fputs\n";
//    fputs(s3,stdout);
//
//    const char *s4="hello fwrite\n";
//    fwrite(s4,strlen(s4),2,stdout);
    //printf("我是一个进程：%d\n",getpid());

    //if(argc!=2)
    //{
    //    printf("Usage:%s filename\n",argv[0]);
    //    return 2;
    //}
    //FILE *fp=fopen(argv[1],"r");
    //if(!fp)
    //{
    //    perror("fopen");
    //    return 1;
    //}

    //while(1)
    //{
    //    char buffer[1024];
    //    buffer[0]=0;
    //    size_t n=fread(buffer,1,sizeof(buffer)-1,fp);
    //    if(n>0)
    //    {
    //        buffer[n]=0;
    //        printf("%s",buffer);
    //    }
    //    else if(feof(fp))
    //        break;

    //}
    ////const char *str="hello,world!\n";
    ////fwrite(str,strlen(str),1,fp);
    //fclose(fp);
    //return 0;
//}
