#pragma once

#include <iostream>
#include <mutex>
#include <pthread.h>

class Mutex
{
private:
    pthread_mutex_t _lock;

public:
    Mutex()
    {
        pthread_mutex_init(&_lock,nullptr);
    }
    ~Mutex()
    {
        pthread_mutex_destroy(&_lock);
    }

    void Lock()
    {
        pthread_mutex_lock(&_lock);
    }

    void Unlock()
    {
        pthread_mutex_unlock(&_lock);
    }

    pthread_mutex_t* Get()
    {
        return &_lock;
    }
};

class LockGuard
{
private:
    Mutex* _mutexp;
public:
    LockGuard(Mutex*_mutex):_mutexp(_mutex)
    {
        _mutexp->Lock();
    }
    ~LockGuard()
    {
        _mutexp->Unlock();
    }
};

