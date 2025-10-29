#pragma once

#include <iostream>
#include<string>
#include<filesystem>    //C++17
#include "Mutex.hpp"

enum class LogLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

std::string LeverlToString(LogLevel level)
{
    switch (level)
    {
    case LogLevel::DEBUG:
        return "Debug";
    case LogLevel::ERROR:
        return "Error";
    case LogLevel::INFO:
        return "Info";
    case LogLevel::WARNING:
        return "Warning";
    case LogLevel::FATAL:
        return "Fatal";

    default:
        return "Unkoown";
    }
}

// 显示器刷新
class SynLogStrategy
{
private:
    Mutex _lock;
public:

};

const std::string logdefaultdir="log";
const static std::string logfilename="test.log";

// 文件刷新
class FileLogStrategy
{
private:
    std::string _dir_path_name; //存储日志文件的文件位置
    std::string _filename;  //日志文件的名称
public:
    FileLogStrategy(const std::string &dir=logdefaultdir,
        const std::string filename=logfilename)
        :_dir_path_name(dir),
        _filename(filename)
    {
        if(std::filesystem::exists(dir))
        {
            return ;
        }
        try{
            //可能出现因为权限问题等创建失败
            std::filesystem::create_directories(_dir_path_name);
        }
        catch(const std::filesystem::filesystem_error&e)
        {
            //返回错误原因的字符串
            std::cerr<<e.what()<<"\r\n";
        }
    }
    ~FileLogStrategy()
    {}
};