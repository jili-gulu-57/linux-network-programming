#include <iostream>
#include <pthread.h>
#include <queue>
#include "Mutex.hpp"
#include "Cond.hpp"

const static u_int32_t gcap = 5;
template <typename T>
class BlockQueue
{
private:
    bool IsFull()
    {
        return _bq.size() >= _cap;
    }
    bool IsEmpty()
    {
        return _bq.empty();
    }

public:
    BlockQueue(u_int32_t cap = gcap) : _cap(cap)
    {
    }

    void Enqueue(const T &in)
    {
        pthread_mutex_lock(&_lock);

        if (IsFull())
        {
            // 队列已满
            // 原子性进入等待，调用pthread_cond_wait之前，线程必须已经获得了互斥锁，pthread_cond_wait会做三件事：
            // 1原子地解放mutex 2将当前线程挂起，进入等待cond状态 3收到信号被唤醒后，重新尝试获取mutex锁
            pthread_cond_wait(&_p_cond, &_lock);
        }
        // 队列不满
        _bq.push(in);
        // 唤醒消费者
        pthread_cond_signal(&_c_cond);
        pthread_mutex_unlock(&_lock);
    }

    void Pop(T *out)
    {
        pthread_mutex_lock(&_lock);
        if (IsEmpty())
        {
            pthread_cond_wait(&_c_cond, &_lock);
        }
        *out = _bq.front();
        _bq.pop();

        pthread_cond_signal(&_p_cond);
        pthread_mutex_unlock(&_lock);
    }
    ~BlockQueue()
    {
    }

private:
    std::queue<T> _bq;
    u_int32_t _cap;
    Mutex _lock;
    Cond _c_cond; // 消费者对应环境变量
    Cond _p_cond; //
};

class LockGuard()
{
public:
    LockGuard()
    {}
    ~LockGuard()
    {

    }
private:
    Mutex *_mutexp;
};