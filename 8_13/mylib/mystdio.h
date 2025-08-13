#ifndef __MYSTDIO_H__
#define __MYSTDIO_H__ 

#define FLUSH_NONE 1  //无刷新
#define FLUSH_LINE 2  //行刷新
#define FLUSH_FULL 4  //全刷新

#define SIZE 4096     //缓冲区大小
#define UMASK 0666    //文件权限掩码

#define FORCE 1     //强制刷新
#define NORMAL 2    //普通刷新

typedef struct _MY_IO_FILE
{
    int fileno;    //文件描述符
    int flag;      //刷新方式
    char outbuffer[SIZE];   //文件输出缓冲区
    int curr;     //当前缓冲区有多少内容
    int cap;      //缓冲区大小
}MyFILE;

//文件打开函数
MyFILE *my_fopen(const char *filename,const char *mode);
                  //打开的文件名称      文件打开方式
//文件关闭函数
void my_fclose(MyFILE *fp);

//文件写入函数
int my_fwrite(const char *s,int size,MyFILE *fp);
//成功写入返回正数，错误返回-1。要写入的字符串，大小，要写入的文件

//文件刷新函数
void my_fflush(MyFILE *fp);
static void my_fflush_core(MyFILE *fp,int force);

#endif

