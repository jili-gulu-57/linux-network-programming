#include"BlockQueue.hpp"

void *consumer(void *args)
{

}

void* productor(void *args)
{

}

int main()
{
    pthread_t c,p;
    pthread_create(&c,nullptr,consumer,(void*)"消费者");
    pthread_create(&p,nullptr,productor,(void*)"生产者");

    pthread_join(c,nullptr);
    pthread_join(p,nullptr);
    return 0;
}