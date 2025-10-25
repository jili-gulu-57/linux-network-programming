#pragma once

#include <iostream>
#include <mutex>
#include <pthread.h>

class Mutex
{
public:
    Mutex()
    {
        pthread_mutex_init(&_lock,nullptr);
    }
    ~Mutex()
    {
        pthread_mutex_destroy(&_lock);
    }
    pthread_mutex_t * Get()
    {
        return &_lock;
    }
    void Lock()
    {
        pthread_mutex_lock(&_lock);
    }
    void Unlock()
    {
        pthread_mutex_unlock(&_lock);
    }

private:
    pthread_mutex_t _lock;
};