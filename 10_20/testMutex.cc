#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<sched.h>
#include"Mutex.hpp"

int tickets=1000;
Mutex lock;

void *route(void *arg)
{
    char *id=(char*)arg;
    while(1)
    {
        
    }
}

int main()
{
    pthread_t t1,t2,t3,t4;

    pthread_create(&t1,nullptr,route,(void*)"thread-1");
    pthread_create(&t2,nullptr,route,(void*)"thread-2");
    pthread_create(&t3,nullptr,route,(void*)"thread-3");
    pthread_create(&t4,nullptr,route,(void*)"thread-4");

    pthread_join(t1,nullptr);
    pthread_join(t2,nullptr);
    pthread_join(t3,nullptr);
    pthread_join(t4,nullptr);
    return 0;

}