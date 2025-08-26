#pragma once

#include<iostream>
#include<string>
#include<functional>
#include<vector>

//4种任务

// using task_t=std::functional<void()>;
using task_t = std::function<void()>;

void Download()
{
    std::cout<<"我是一个下载任务"<<std::endl;
}

void MySql()
{
    std::cout<<"我是一个MySql任务"<<std::endl;
}

void Sync()
{
    std::cout<<"我是一个数据刷新同步任务"<<std::endl;
}

void Log()
{
    std::cout<<"我是一个日志保存任务"<<std::endl;
}

std::vector<task_t> tasks;

class Init
{
public:
    Init()
    {
        tasks.push_back(Download);
        tasks.push_back(MySql);
        tasks.push_back(Sync);
        tasks.push_back(Log);
    }
    ~Init()
    {}

private:
    /* data */
};

Init ginit; //定义一个全局变量，自动调用初始化函数