#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <cstdio>

void *thread_routine(void *args)
{
    std::string name = (const char *)args;
    while (true)
    {
        std::cout << "我是新线程,名字是：" << name << std::endl;
        sleep(1);
    }
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, thread_routine, (void *)"thread-1"); // 最后一个参数会传递给新线程，作为参数
    while (true)
    {
        std::cout << "我是主线程" << std::endl;
        sleep(1);
    }
}