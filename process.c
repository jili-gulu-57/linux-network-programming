#include "process.h"
#include <string.h>
#include<unistd.h>
#define SIZE 101
#define STYLE '='

//version 2:根据current和total打印进度
void FlushProcess(double total,double current)
{
  const char* lable="|/-\\";//条状进度条
  static int index=0;
  int len=strlen(lable);
  char buffer[SIZE];
  memset(buffer,0,sizeof(buffer));
  double rate=current/total*100;
  int num=(int)(rate);//显示打印‘=的个数
  int i=0;
  for(i=0;i<num;i++)
    buffer[i]=STYLE;

  printf("[%-100s] %.1lf%% %c \r ",buffer,rate,lable[index++ % len]);
  fflush(stdout);
}

//version 1:仅作为进度条展示
//void process()
//{
//  int rate=0;
//  char buffer[SIZE];
//  memset(buffer,0,sizeof(buffer));
//  const char *lable="|/-\\";
//  int len=strlen(lable);
//  while(rate<=100)
//  {
//    printf("[%-100s] %d%% %c \r",buffer,rate,lable[rate%len]);
//    fflush(stdout);
//    buffer[rate]=STYLE;
//    rate++;
//    usleep(100000);
//  }
//  printf("\n");
//}
