#include <iostream>
#include <pthread.h>
#include<queue>

const static u_int32_t gcap=5;
template<typename T>
class BlockQueue
{
    private:
    bool IsFull()
    {
        return _bq.size()>=_cap;
    }
public:
    BlockQueue(u_int32_t cap=gcap):_cap(cap)
    {
        pthread_mutex_init(&_lock,nullptr);
        pthread_cond_init(&_c_cond,nullptr);
        pthread_cond_init(&_p_cond,nullptr);
    }
    void Enqueue(const T &in)
    {
        pthread_mutex_lock(&_lock);
        pthread_mutex_unlock(&_lock);

        if(IsFull())
        {
            //队列已满

        }
    }
    void Pop(T*out)
    {

    }
    ~BlockQueue()
    {
        pthread_mutex_destroy(_lock);
        pthread_cond_destroy(&_c_cond);
        pthread_cond_destroy(&_p_cond);
    }

private:
    std::queue<T> _bq;
    u_int32_t _cap;
    pthread_mutex_t _lock;
    pthread_cond_t _c_cond; //消费者对应环境变量
    pthread_cond_t _p_cond; //
};