#include<iostream>
#include<pthread.h>
#include<string>
#include<unistd.h>

pthread_cond_t gcond=PTHREAD_COND_INITIALIZER;
pthread_mutex_t gmutex=PTHREAD_MUTEX_INITIALIZER;

void *active(void *args)
{
    std::string name=static_cast<const char*> (args);
    while(true)
    {
        pthread_mutex_lock(&gmutex);
        pthread_cond_wait(&gcond,&gmutex);
        std::cout<<name<<":active"<<std::endl;
        pthread_mutex_unlock(&gmutex);
    }
    return (void*)0;
}

int main()
{
    pthread_t t1,t2,t3,t4;
    pthread_create(&t1,nullptr,active,(void*)"thread-1");
    pthread_create(&t2,nullptr,active,(void*)"thread-2");
    pthread_create(&t3,nullptr,active,(void*)"thread-3");
    pthread_create(&t4,nullptr,active,(void*)"thread-4");

    sleep(3);
    while(true)
    {
        //每次唤醒一个进程
        // pthread_cond_signal(&gcond);

        //一次唤醒所有进程
        pthread_cond_broadcast(&gcond);
        sleep(1);
    }

    pthread_join(t1,nullptr);
    pthread_join(t2,nullptr);
    pthread_join(t3,nullptr);
    pthread_join(t4,nullptr);
    return 0;
}