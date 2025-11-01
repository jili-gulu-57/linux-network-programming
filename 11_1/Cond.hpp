#pragma once

#include <iostream>
#include <mutex>
#include <pthread.h>
#include "Mutex.hpp"

class Cond
{
private:
    /* data */
    pthread_cond_t _cond;

public:
    Cond(/* args */)
    {
        pthread_cond_init(&_cond, nullptr);
    }
    ~Cond()
    {
        pthread_cond_destroy(&_cond);
    }
    void Wait(Mutex &lock)
    {
        int n = pthread_cond_wait(&_cond, lock.Get());
    }
    void NotifyOne()
    {
        int n = pthread_cond_signal(&_cond);
        (void *)n;
    }
    void NotifyAll()
    {
        int n = pthread_cond_broadcast(&_cond);
        (void *)n;
    }
};
