#include <iostream>
#include <queue>
#include <vector>
#include "Mutex.hpp"
#include "Thread.hpp"
#include "Cond.hpp"

// const表示该量是常量，不可修改；static避免重复定义
const static int defaultthreadnum = 3; // 默认线程个数

template <class T>
class ThreadPool
{
private:
    // 任务队列
    std::queue<T> _q;
    // 多个线程
    std::vector<Thread> _threads;

    Mutex _lock;
    Cond _c;
    int _threadnum; // 线程池线程数量
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

public:
    ThreadPool(int threadnum = defaultthreadnum) : _threadnum = threadnum, _is_running(false)
    {
        // 创建线程
        for (int i = 0; i < _threadnum; i++)
        {
            // 方法1：
            auto f = std::bind(hello, this);

            // 方法2：lambda表达式
            _threads.emplace_back([this]()
                                  { this->hello(); }, name);

            // std::string name="thread-"+std::to_string(i+1);
            // Thread t([this]()
            //          { this->hello(); }, name);

            // _threads.push_back(std::move(t));
        }
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
};
