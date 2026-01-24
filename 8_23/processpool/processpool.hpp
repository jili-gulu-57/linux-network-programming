// 定义声明在一起，常用于开源库
#ifndef __PROCESS_POOL_HPP__
#define __PROCESS_POOL_HPP__

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <string>
#include <vector>
#include <functional>
#include <cstdlib>
#include <sys/wait.h>
#include <ctime>
#include "task.hpp"

const int gdefault_process_num = 5; // 子进程数量

using callback_t = std::function<void(int)>;

// 用C++类来封装进程池的管理逻辑（启动，分发任务，回收进程）
class Channel
{
public:
    Channel() // 无参构造
    {
    }

    Channel(int fd, const std::string &name, pid_t id)
        : _wfd(fd),
          _name(name),
          _sub_target(id)
    {
    }

    void DebugPrint()
    {
        printf("channel name:%s,fd:%d,id:%d\n", _name.c_str(), _wfd, _sub_target);
    }

    ~Channel() {}

    int Fd() { return _wfd; }
    std::string Name() { return _name; }
    pid_t Target() { return _sub_target; }
    void Close() { close(_wfd); }
    void Wait()
    {
        pid_t rid = waitpid(_sub_target, nullptr, 0);
        (void)rid;
    }

private:
    int _wfd;          // 类内属性统一带“_”作为区分
    std::string _name; // 为通道起名字
    pid_t _sub_target; // 子进程
};

// 先创建管道，后创建进程
class ProcessPool
{
public:
    ProcessPool(int num = gdefault_process_num) : _processnum(num)
    {
        srand(time(nullptr) ^ getpid() ^ 0x567);
    }
    ~ProcessPool()
    {
    }

    bool InitProcessPool(callback_t cb)
    {
        for (int i = 0; i < _processnum; i++)
        {
            // 1.创建管道
            int pipefd[2] = {0};
            int n = pipe(pipefd);
            if (n < 0)
                return false;
            (void)n;

            // 2.创建子进程（执行main函数的进程就是父进程）
            pid_t id = fork();
            if (id < 0)
                return false;
            if (id == 0)
            {
                // 子进程，父写子读
                //除了关闭写端，还要关闭从父进程继承的写端
                for(auto &c:_channels)
                {
                    c.Close();
                }
                
                close(pipefd[1]);
                // sleep(10);

                // todo
                cb(pipefd[0]);

                // 子进程无需往后执行代码，退出即可
                exit(0);
            }

            // 父进程
            close(pipefd[0]);
            std::string name = "channel-" + std::to_string(i);
            _channels.emplace_back(pipefd[1], name, id); // 直接在vector内部创建对象

            // Channel channel(pipefd[1],name,id);
            // Channels.push_back(channel);
        }
        return true;
    }

    // //1.创建进程池，让子进程进入自己的工作流中，父进程得到channels
    // InitProcessPool(_channels,[](int fd){

    // for(auto c:_channels)
    // {
    //     c.DebugPrint();
    // }

    // sleep(2);
    // std::cout<<"父进程开始控制"<<std::endl;

    // 2.管理子进程
    // 2.1轮询选择一个进程
    void PollingCtrlSubProcess()
    {
        int index = 0;
        while (true)
        {
            CtrlSubProcessHelper(index);
        }
    }

    void PollingCtrlSubProcess(int count) // 控制次数
    {
        if (count < 0)
            return;
        int index = 0;
        while (count)
        {
            CtrlSubProcessHelper(index);
            count--;
        }
    }

    void WaitSubProcess()
    {
        for(auto &c:_channels)
        {
            c.Close();
            c.Wait();
        }


        // for (auto &c : _channels)
        //     c.Close();
        // for (auto &c : _channels)
        // {
        //     c.Wait();
        //     printf("子进程%d被回收\n", c.Target());
        // }
    }

private:
    std::vector<Channel> _channels; // 所有信道
    int _processnum;                // 子进程数量
        void CtrlSubProcessHelper(int &index)
    {
        // 选择一个信道
        int who = index;
        index++;
        index %= _channels.size();

        // 随机选择一个任务
        int x = rand() % tasks.size(); // 测试
        std::cout << "选择信道：" << _channels[who].Name() << ",subtarget:" << _channels[who].Target() << std::endl;
        write(_channels[who].Fd(), &x, sizeof(x));
        sleep(1);
    }
};

#endif