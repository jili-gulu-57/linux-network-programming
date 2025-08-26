#include"processpool.hpp"
#include<memory> 

int main()
{
    //1.创建进程池
    ProcessPool pp;

    //2.初始化进程池
    pp.InitProcessPool([](int fd){
        while(true)
        {
            int code=0;
            // std::cout<<"子进程被阻塞："<<getpid()<<std::endl;
            ssize_t n=read(fd,&code,sizeof(code));
            if(n==sizeof(code))
            {
                std::cout<<"子进程运行："<<getpid()<<std::endl;\
                if(code>=0&&code<tasks.size())\
                {
                    tasks[code]();
                }
                else
                {
                    std::cerr<<"任务码错误"<<code<<std::endl;
                    break;
                }
            }
            else if(n==0)    //管道关闭，子进程应退出
            {
                // std::cout<<"写端关闭，子进程退出"<<getpid()<<std::endl;
                printf("写端关闭，子进程%d退出\n",getpid());
                break;
            }
            else
            {
                std::cerr<<"read fd:"<<fd<<" error"<<std::endl;
                break;
            }
        }  
    });

    //3.控制进程池
    pp.PollingCtrlSubProcess(6);

    //4.进程池
    pp.WaitSubProcess();

    std::cout<<"父进程控制子进程完成,父进程结束"<<std::endl;

    return 0;
}