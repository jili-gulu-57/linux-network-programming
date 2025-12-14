#pragma once

#include <iostream>
#include <mutex>
#include <pthread.h>
#include "Mutex.hpp"

class Cond
{
private:
    pthread_cond_t _cond;

public:
    Cond()
    {
        pthread_cond_init(&_cond, nullptr);//初始化条件变量，第二个参数为属性（nullptr表示默认属性）
    }

    void Wait(Mutex &lock)
    {
        int n = pthread_cond_wait(&_cond, lock.Get());// 等待条件变量
        // 参数：条件变量和互斥锁（lock.Get()返回pthread_mutex_t*）
        // 调用时会自动释放互斥锁，然后线程进入等待状态
        // 当被唤醒后，会自动重新获取互斥锁
        
        (void *)n;  // 将返回值强制转换为void*，避免未使用变量的警告
        // 更好的做法是：处理返回值检查错误
    }

    void NotifyOne()
    {
        int n = pthread_cond_signal(&_cond);// 唤醒至少一个等待该条件变量的线程
        (void *)n;
    }
    void NotifyAll()
    {
        int n = pthread_cond_broadcast(&_cond);// 唤醒所有等待该条件变量的线程
        (void *)n;
    }

    ~Cond()
    {
        pthread_cond_destroy(&_cond);// 销毁条件变量，释放资源
    }
};
