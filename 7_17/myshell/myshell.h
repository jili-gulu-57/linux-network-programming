#ifndef __MYSHELL_H__
#define __MYSHELL_H__ 

#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define ARGS 64

//命令行参数表，定义成全局，让子进程可以继承
//extern char* gargv[];

void Debug();
void InitGlobal();
void PrintCommandPrompt();
bool GetCommandString(char cmd_str_buff[],int len);
bool ParseCommandString(char cmd[]);
void ForkAndExec();
bool BuiltInCommandExec();
#endif 
