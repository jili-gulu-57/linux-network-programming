#include "myshell.h"

#define SIZE 1024
int main()
{
    char commandstr[SIZE]; //获取用户输入的命令
    while(true)
    {
        //1.输出命令行提示符
        PrintCommandPrompt();
        //2.获取用户输入的命令
        GetCommandString(commandstr,SIZE);
        //3.解析命令行内容
        ParseCommandString(commandstr);
        //4.检查是否为内建命令，shell自己执行
        if(BuiltInCommandExec())
        {
            continue;
        }
        //5.子进程执行命令行
        ForkAndExec();
    }
    return 0;
}
