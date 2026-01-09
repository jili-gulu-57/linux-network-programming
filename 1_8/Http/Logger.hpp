// 用户使用LOG宏 → 创建LogMessage对象 → 流式拼接日志内容 → 对象析构时自动刷新 → 通过策略输出到目标
#pragma once

#include <iostream>
#include <string>
#include <filesystem> //C++17
#include <fstream>
#include <ctime>
#include <unistd.h>
#include "Mutex.hpp"

// 日志级别枚举
enum class LogLevel
{
    DEBUG,   // 调试
    INFO,    // 信息
    WARNING, // 警告
    ERROR,   // 错误
    FATAL    // 重大错误
};

// 将枚举值转换为可读字符串
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
        return "Unkown";
    }
}

// 获取当前时间
std::string GetCurrentTime()
{
    // 获取时间戳
    time_t currtime = time(nullptr);

    // 时间戳转换为20XX-10-30 10:32:57
    struct tm currtm;
    localtime_r(&currtime, &currtm);

    // 转换成字符串
    char timebuffer[64];
    snprintf(timebuffer, sizeof(timebuffer), "%4d-%02d-%02d %02d:%02d:%02d",
             currtm.tm_year + 1900, currtm.tm_mon + 1, currtm.tm_mday,
             currtm.tm_hour, currtm.tm_min, currtm.tm_sec);

    return timebuffer;
}

// 策略模式基类方法
class LogStrategy
{
public:
    virtual ~LogStrategy() = default;   //虚析构函数
    virtual void SyncLog(const std::string &logmessage) = 0;    //纯虚函数
};

// 显示器刷新
class ConsoleLogStrategy : public LogStrategy
{
private:
    Mutex _lock;    //互斥锁，保证多线程安全

public:
    ~ConsoleLogStrategy()
    {
    }
    void SyncLog(const std::string &logmessage) override
    {
        {
            LockGuard lockguard(&_lock);
            std::cout << logmessage << std::endl;   //输出到终端
        }
    }
};

const std::string logdefaultdir = "log";
const static std::string logfilename = "test.log";

// 文件刷新
class FileLogStrategy : public LogStrategy
{
private:
    std::string _dir_path_name; // 存储日志文件的文件位置
    std::string _filename;      // 日志文件的名称
    Mutex _lock;

public:
    //构造函数，可指定文件位置和日志文件名称
    FileLogStrategy(const std::string &dir = logdefaultdir,
                    const std::string filename = logfilename)
        : _dir_path_name(dir),
          _filename(filename)
    {
        LockGuard lockguard(&_lock);
        if (std::filesystem::exists(dir))   //目录已存在则返回
        {
            return;
        }
        try
        {
            // 可能出现因为权限问题等创建失败
            std::filesystem::create_directories(_dir_path_name);//创建目录
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            // 返回错误原因的字符串
            std::cerr << e.what() << "\r\n";
        }
    }
    void SyncLog(const std::string &logmessage) override
    {
        {
            LockGuard lockguard(&_lock);
            std::string target = _dir_path_name;
            target += "/";
            target += _filename;    //完整文件路径

            std::ofstream out(target.c_str(), std::ios::app);//以追加方式打开文件
            if (!out.is_open())
            {
                return;
            }
            out << logmessage << "\n";  //写入文件
            out.close();    //关闭文件
        }
    }
    ~FileLogStrategy()
    {
    }
};

/////////////////////////////////////////////////////////////

//主日志类

// 1.定制刷新策略
// 2.构建完整日志

class Logger
{
private:
    std::unique_ptr<LogStrategy> _strategy;//使用智能指针管理策略对象

public:
    Logger()
    {
    }

    //启用终端输出策略
    void EnableConsoleLogStrategy()
    {
        _strategy = std::make_unique<ConsoleLogStrategy>();
    }

    //启动文件输出策略
    void EnableFileLogStrategy()
    {
        _strategy = std::make_unique<FileLogStrategy>();
    }

    class LogMessage
    {
    public:
        //初始化日志基本信息
        LogMessage(LogLevel level, std::string &filename, int line, Logger &logger)
            : _curr_time(GetCurrentTime()),
              _level(level),
              _pid(getpid()),
              _filename(filename),
              _line(line),
              _logger(logger)
        {
            // 构建日志头部：[时间][级别][PID][文件名][行号] - 
            std::stringstream ss;
            ss << "[" << _curr_time << "]"
               << "[" << LeverlToString(_level) << "]"
               << "[" << _pid << "]"
               << "[" << filename << "]"
               << "[" << line << "]"
               << " - ";
            _loginfo = ss.str();
        }

        //重载"<<"运算符，直接链式调用
        template <typename T>
        LogMessage &operator<<(const T &info)
        {
            std::stringstream ss;
            ss << info;
            _loginfo += ss.str();
            return *this;
        }

        ~LogMessage()
        {
            if (_logger._strategy)
            {
                _logger._strategy->SyncLog(_loginfo);
            }
        }

    private:
        std::string _curr_time;
        LogLevel _level;
        pid_t _pid;
        std::string _filename;
        int _line;
        std::string _loginfo; // 一条合并好的，完整的日志信息
        Logger &_logger;
    };

    //重载"()"运算符，构造LogMessage对象
    LogMessage operator()(LogLevel level, std::string filename, int line)
    {
        return LogMessage(level, filename, line, *this);
    }
    
    ~Logger()
    {
    }
};

Logger logger;

#define LOG(level) logger(level, __FILE__, __LINE__)
#define EnableConsoleLogStrategy() logger.EnableConsoleLogStrategy()
#define EnableFileLogStrategy() logger.EnableFileLogStrategy()
