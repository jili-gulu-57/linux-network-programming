#include <iostream>
#include <unistd.h>
#include <pthread.h>

volatile int gval = 100;

void *thread_routine(void *args)
{
    std::string threadname = static_cast<const char *>(args);
    while (true)
    {
        std::cout << "new thread"<<std::endl;
        printf("gval:%d,&gval:%p\n", gval, &gval);
        gval++;
        sleep(1);
        // break;
    }
    // return nullptr;  //第一种退出方法
    // exit(10);   //exit是用来终止进程，任何一个线程调用则会导致整个进程退出，即所有线程退出  
    // pthread_exit(nullptr);  //第二种退出方法
}

int main()
{
    pthread_t tid;
    int n = pthread_create(&tid, nullptr, thread_routine, (void *)"thread -l");
    (void)n;

    // int m=pthread_join(tid,nullptr);
    // (void)m;
    sleep(3);
    printf("main thread\n");

    //第三种，主线程取消其他线程
    pthread_cancel(tid);

    // while (true)
    // {
    //     std::cout << "main thread";
    //     printf("gval:%d,&gval:%p\n", gval, &gval);
    //     sleep(1);
    // }
    return 0;
}
