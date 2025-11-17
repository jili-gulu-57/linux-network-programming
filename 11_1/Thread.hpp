#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include <iostream>
#include <unistd.h>
#include <vector>
#include <pthread.h>
#include <functional>
#include <sys/syscall.h>

// 宏定义，用get_lwp_id()代替syscall(SYS_gettid)
#define get_lwp_id() syscall(SYS_gettid)
// syscall 是 Linux 中直接调用系统调用的函数。
// SYS_gettid 是一个代表系统调用号的常量，对应的是 gettid 系统调用。
// gettid 系统调用返回当前调用线程的 线程ID（TID）。

// C++类型别名声明
using func_t = std::function<void()>;
// using func_t :类型别名语法，相当于typedef
// std::function<void()>：函数对象类型

const std::string threadnamefault = "None-name"; // 默认线程名称

class Thread
{
private:
    pthread_t _tid;    // 线程库级别id，用于线程库内部的线程管理
    pid_t _lwpid;      // Linux内核线程id，用于内核级别操作和系统调用
    std::string _name; // 线程名称
    func_t _func;      // 线程执行函数
    bool _isrunning;   // 线程运行状态标识

public:
    Thread(func_t func, const std::string &name = threadnamefault)
        : _func(func),
          _name(name),
          _isrunning(false)
    {
        std::cout << "creat thread obj success" << std::endl;
    };

    // 用static修饰，否则会带this指针
    static void *start_routine(void *args)
    {
        Thread *self = static_cast<Thread *>(args);
        self->_isrunning = true;
        self->_lwpid = get_lwp_id();
        self->_func();
        pthread_exit((void *)0);
    }

    void Start()
    {
        int n = pthread_create(&_tid, nullptr, start_routine, this);
        if (n == 0)
        {
            printf("create thread success\n");
        }
    }

    void Stop()
    {
        
    }

    void Join()
    {
        int n = pthread_join(_tid, nullptr);
        if (n == 0)
        {
            std::cout << "join success" << std::endl;
        }
    }
    ~Thread()
    {
    }
};

#endif