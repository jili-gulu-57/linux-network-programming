#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
extern char** environ;
int g_unval;
int g_val=100;
int main(int argc,char *argv[])
{
    printf("code address:%p\n",main);
    printf("init val address:%p\n",&g_val);
    printf("uninit val address:%p\n",&g_unval);
    //(void)argc;
    //(void)argv;

    //for(int i=0;environ[i];i++)
    //{
    //    printf("environ[%d]->%s\n",i,environ[i]);
    //}
    //const char* who=getenv("USER");
    //if(who==NULL)
    //    return 1;
    //
    //if(strcmp(who,"wq")==0)
    //    printf("正常执行逻辑\n");
    //else 
    //    printf("ONLY WQ!\n");

    //char *value=getenv("PATH");
    //if(value==NULL)
    //    return 1;
    //printf("PATH->%s\n",value);



   // for(int i=0;env[i];i++)
   // {
   //     printf("env[%d]->%s\n",i,env[i]);
   // }

    //if(argc!=2)
    //{
    //    printf("error: %s [-a/-b/-c]\n",argv[0]);
    //    return 1;
    //}
    //const char *arg=argv[1];
    //if(strcmp(arg,"-a")==0)
    //    printf("功能1\n");
    //else if (strcmp(arg,"-b")==0)
    //    printf("功能2\n");
    //else if (strcmp(arg,"-c")==0)
    //    printf("功能3\n");
    //else 
    //    printf("没有该功能\n");


    //for(int i=0;i<argc;i++)
    //{
    //    printf("argv[%d]:%s\n",i,argv[i]);
    //}
    return 0;
}

