#include <iostream>
#include <unistd.h>
#include <pthread.h>

volatile int gval = 100;

void *thread_routine(void *args)
{
    std::string threadname = static_cast<const char *>(args);
    while (true)
    {
        std::cout << "new thread";
        printf("gval:%d,&gval:%p\n", gval, &gval);
        gval++;
        sleep(1);
    }
    
}

int main()
{
    pthread_t tid;
    int n = pthread_create(&tid, nullptr, thread_routine, (void *)"thread -l");
    (void)n;

    while (true)
    {
        std::cout << "main thread";
        printf("gval:%d,&gval:%p\n", gval, &gval);
        sleep(1);
    }
    return 0;
}
