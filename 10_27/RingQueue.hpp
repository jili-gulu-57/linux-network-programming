#pragma once

#include <iostream>
#include <vector>
#include<pthread.h>
#include"Sem.hpp"
#include"Mutex.hpp"

static int gcap = 5;

template <typename T>
class RingQueue
{
private:
    int _cap;
    std::vector<T> _ring_queue;
    Sem _space_sem;
    Sem _data_sem;

    int _p_step;
    int _c_step;
    
    Mutex _c_lock;
    Mutex _p_lock;
public:
    RingQueue(int cap = gcap) 
    : _cap(cap),_ring_queue(cap),_space_sem(cap),_data_sem(0),_p_step(0),_c_step(0)
    {}
    ~RingQueue()
    {}
    void Pop(T*out)
    {
        _data_sem.P();
        *out=_ring_queue[_c_step++];
        _c_step%=_cap;
        _space_sem.V();
    }
    void Enqueue(const T&in)
    {
        _space_sem.P();
        _ring_queue[_p_step++]=in;
        _p_step%=_cap;
        _data_sem.V();
    }
};