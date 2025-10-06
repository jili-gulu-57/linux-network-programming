#include<iostream>
#include<unistd.h>
#include<pthread.h>

void *thread_routine(void * name)
{
    std::cout<<"new thread"<<std::endl;
    sleep(1);
}

int main()
{
    pthread_t tid;
    pthread_create(&tid,nullptr,thread_routine,(void *)"thread -l");

    while(true)
    {
        std::cout<<"main thread"<<std::endl;
        sleep(1);
    }
    return 0;
}
