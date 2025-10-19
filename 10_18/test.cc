#include <cstdio>
#include <unistd.h>
#include <pthread.h>
#include <string>

// 线程并不安全，因为线程执行代码，将内存数据tickets拷贝到CPU寄存器时，就相当于全局变量变成自己私有变量
int tickets = 1000;
pthread_mutex_t gmutex = PTHREAD_MUTEX_INITIALIZER;

void *routine(void *args)
{
    std::string name = static_cast<const char *>(args);
    while (true)
    {
        pthread_mutex_lock(&gmutex);
        if (tickets > 0)
        {
            usleep(10000);
            printf("%s票号为：%d\n", name.c_str(), tickets--);
            pthread_mutex_unlock(&gmutex); 
        }
        else
        {
            pthread_mutex_unlock(&gmutex);
            break;
        }
    }
    return (void *)0;
}

int main()
{
    pthread_t t1, t2, t3, t4;
    pthread_create(&t1, nullptr, routine, (void *)"thread-1");
    pthread_create(&t2, nullptr, routine, (void *)"thread-2");
    pthread_create(&t3, nullptr, routine, (void *)"thread-3");
    pthread_create(&t4, nullptr, routine, (void *)"thread-4");

    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    pthread_join(t3, nullptr);
    pthread_join(t4, nullptr);
    return 0;
}