#include <iostream>
#include <functional>
#include <vector>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

// 打印pending信号
void PrintPending(sigset_t &pending)
{
    printf("[pid:%d] pending list:", getpid());
    for (int signo = 31; signo > 0; signo--)
    {
        if (sigismember(&pending, signo))
            std::cout << "1";
        else
            std::cout << "0";
    }
    std::cout << "\r\n";
}
int main()
{
    
    // 屏蔽2号信号
    sigset_t block, oblock;
    sigemptyset(&block);
    sigemptyset(&oblock);

    // 在用户层面设置2号屏蔽
    // sigaddset(&block, 2);

    // 设置到内核层面
    sigprocmask(SIG_SETMASK, &block, &oblock);

    while(true)
    {
        sigset_t pending;
        sigemptyset(&pending);
        sigpending(&pending);   //获取当前进程的pending集
        PrintPending(pending); //打印pending集的信号
        sleep(1);
    }
    return 0;
}