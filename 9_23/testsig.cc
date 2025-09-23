#include<iostream>
#include<unistd.h>
#include<signal.h>

void handler(int signo)
{
    std::cout<<"捕捉到了信号："<<signo<<std::endl;
}

int main()
{
    struct sigaction act,oact;
    act.sa_handler=handler;
    act.sa_flags=0;
    sigemptyset(&(act.sa_mask));
    sigaction(2,&act,&oact);    //将2号信号的处理方法设置到了内核中
    
    while(true)
    {
        std::cout<<"我是进程"<<getpid()<<std::endl;
        sleep(1);
    }
    return 0;
}