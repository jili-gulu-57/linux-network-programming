#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include<unistd.h>
#include "Mutex.hpp"
#include "Thread.hpp"
#include "Cond.hpp"

//线程池是通过预先创建一批线程来处理任务，并重复利用，减少频繁创建、销毁线程的开销。
//1.任务队列：①待处理的任务（通常用队列或链表实现）②包含任务添加和取出接口
//2.线程管理器：①创建和销毁线程 ②管理线程数量 ③监管线程状态
//3.工作线程：①从任务队列获取任务并执行 ②空闲时等待新任务 ③执行完任务线程不销毁，等待下一个
//4.任务接口：①定义任务执行的标准接口 ②通常包含回调函数或函数指针

// const表示该量是常量，不可修改；static避免重复定义
const static int defaultthreadnum = 3; // 默认线程个数

template <class T>
class ThreadPool
{
private:
    bool QueueIsEmpty()
    {
        return _q.empty();
    }

    void Routine(const std::string&name)
    {
        while(true)
        {
            //把任务从线程获取到线程私有
            T t;
            {
                LockGuard lockguard(&_lock);
                while(QueueIsEmpty()&&_is_running)
                {
                    _wait_thread_num++;
                    _cond.Wait(_lock);
                    _wait_thread_num--;
                }
                if(!_is_running&&QueueIsEmpty())
                {
                    LOG(LogLevel::INFO)<<"线程池退出&&任务队列为空，"<<name<<"退出"<<std::endl;
                    break;
                }
                t=_q.front();
                _q.pop();
            }
            t();
            LOG(LogLevel::DEBUG)<<name<<"handler task:"<<t.Result2String();
        }
    }
public:
    //初始化：创建线程
    ThreadPool(int threadnum = defaultthreadnum) 
    : _threadnum = threadnum,
     _is_running(false),
     _wait_thread_num(0)
    {
        // 创建线程
        for (int i = 0; i < _threadnum; i++)
        {
            // 方法1：
            // auto f = std::bind(hello, this);

            // 方法2：lambda表达式
            _threads.emplace_back([this]()
                                  { this->hello(); }, name);

            // std::string name="thread-"+std::to_string(i+1);
            // Thread t([this]()
            //          { this->hello(); }, name);

            // _threads.push_back(std::move(t));
        }
        LOG(LogLevel::INFO)<<"thread pool running success";
    }
    ~ThreadPbool()
    {
    }
    void Start()
    {
        if (_is_running)
            return;

        _is_running = true;
        for (auto &t : _threads)
            t.Start();
    }
    void Stop()
    {
        if (!_is_running)
            return;          // 线程未启动，直接返回
        _is_running = false; // 防止重复停止进程

        for (auto &t : _threads)
            t.Stop();
    }

    void Wait() // 等待所有线程
    {
        for (auto &t : _threads)
            t.Join();
    }

private:
    // 任务队列
    std::queue<T> _q;
    // 多个线程
    std::vector<Thread> _threads;

    Mutex _lock;
    Cond _c;
    int _threadnum; // 线程池线程数量
    int _wait_thread_num;
    bool _is_running;
    void hello()
    {
        // for test
        while (true)
        {
            std::cout << "hello,world" << std::endl;
            sleep(1);
        }
    }
};
