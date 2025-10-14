#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include <iostream>
#include <unistd.h>
#include <vector>
#include <pthread.h>
#include <functional>

// C++类型别名声明
using func_t = std::function<void()>;
// using func_t :类型别名语法，相当于typedef
// std::function<void()>：函数对象类型

const std::string threadnamefault = "None-name";
class Thread
{
private:
    /* data */
    pthread_t _tid;
    pid_t _lwpid;
    std::string _name;
    func_t _func;

public:
    Thread(func_t func, const std::string &name=threadnamefault) : _func(func), _name(name) {};
    void Start()
    {
        int n=pthread_create(&_tid)
    }
    void Join()
    {
    }
    ~Thread();
};

Thread::Thread(/* args */)
{
}

Thread::~Thread()
{
}

#endif