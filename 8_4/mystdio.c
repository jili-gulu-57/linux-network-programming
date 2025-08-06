#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "mystdio.h"
//文件打开函数
MyFILE* my_fopen(const char *filename,const char *mode)
{
     int fd=-1;
    if(strcmp(mode,"a")==0)
    {
        //以追加方式打开
        fd=open(filename,O_CREAT|O_APPEND|O_WRONLY,UMASK);
    }
    else if(strcmp(mode,"r")==0)
    {
        //以只读方式打开
        fd=open(filename,O_RDONLY);
    }
    else if(strcmp(mode,"w")==0)
    {
        //以写方式打开
        fd=open(filename,O_WRONLY|O_CREAT|O_TRUNC,UMASK);
    }
    else if(strcmp(mode,"a+")==0)
    {
        //以读写追加方式打开
        fd=open(filename,O_CREAT|O_RDWR|O_APPEND,UMASK);
    }

    //文件打开失败
    if(fd<0)
    {
        return NULL;
    }
    //文件打开成功
    MyFILE* fp=(MyFILE*)malloc(sizeof(MyFILE));
    if(!fp)
        return NULL;
    fp->fileno=fd;
    fp->flag=FLUSH_LINE;
    fp->curr=0;
    fp->cap=SIZE;
    fp->outbuffer[0]=0;
    return fp;
}

//文件写入函数
int my_fwrite(const char* s,int size,MyFILE *fp)
{
    //将字符串拷到文件缓冲区
    memcpy(fp->outbuffer+fp->curr,s,size);
    fp->curr+=size;
    my_fflush_core(fp,NORMAL);
    return SIZE;
}

//文件刷新函数
static void my_fflush_core(MyFILE *fp,int force)
{
    if(fp->curr<=0)
        return ;
    //开始刷新，检测刷新条件
    if(force==FORCE)
    {
        //强制刷新
        write(fp->fileno,fp->outbuffer,fp->curr);
        fp->curr=0;
    }
    else 
    {
        if((fp->flag&FLUSH_LINE)&&fp->outbuffer[fp->curr-1]=='\n')
        {
            //行刷新
            write(fp->fileno,fp->outbuffer,fp->curr);
            fp->curr=0;
        }
        else if((fp->flag&FLUSH_FULL)&&fp->curr==fp->cap)
        {
            //全刷新
            write(fp->fileno,fp->outbuffer,fp->curr);
            fp->curr=0;
        }
        else 
        {
            write(fp->fileno,fp->outbuffer,fp->curr);
            fp->curr=0;
         }
    }
}

void my_fflush(MyFILE *fp)
{
    //直接调用此函数即为强制刷新
    my_fflush_core(fp,FORCE);
}


//文件关闭函数
void my_fclose(MyFILE *fp)
{
    if(fp->fileno>=0)
    {
        my_fflush(fp);  //用户->C
        fsync(fp->fileno);  //C->内核
        close(fp->fileno);
        free(fp);
    }
}
