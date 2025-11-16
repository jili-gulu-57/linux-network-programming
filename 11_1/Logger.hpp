#pragma once

#include <iostream>
#include <string>
#include <filesystem> //C++17
#include <fstream>
#include <ctime>
#include <unistd.h>
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

// 基类方法
class LogStrategy
{
public:
    virtual ~LogStrategy() = default;
    virtual void SyncLog(const std::string &logmessage) = 0;
};

// 显示器刷新
class ConsoleLogStrategy : public LogStrategy
{
private:
    Mutex _lock;

public:
    ~ConsoleLogStrategy()
    {
    }
    void SyncLog(const std::string &logmessage) override
    {
        {
            LockGuard lockguard(&_lock);
            std::cout << logmessage << std::endl;
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
    FileLogStrategy(const std::string &dir = logdefaultdir,
                    const std::string filename = logfilename)
        : _dir_path_name(dir),
          _filename(filename)
    {
        LockGuard lockguard(&_lock);
        if (std::filesystem::exists(dir))
        {
            return;
        }
        try
        {
            // 可能出现因为权限问题等创建失败
            std::filesystem::create_directories(_dir_path_name);
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
            target += _filename;

            std::ofstream out(target.c_str(), std::ios::app);
            if (!out.is_open())
            {
                return;
            }
            out << logmessage << "\n";
            out.close();
        }
    }
    ~FileLogStrategy()
    {
    }
};

//////////////////////////////////////////////////////////

// 1.定制刷新策略
// 2.构建完整日志

class Logger
{
private:
    std::unique_ptr<LogStrategy> _strategy;

public:
    Logger()
    {}
    ~Logger()
    {}
    class LogMessage
    {
    public:
        LogMessage(LogLevel level, std::string &filename, int line, Logger &logger)
            : _curr_time(GetCurrentTime()),
              _level(level),
              _pid(getpid()),
              _filename(filename),
              _line(line),
              _logger(logger)
        {
            std::stringstream ss;
            ss << "[" << _curr_time << "]"
               << "[" << LeverlToString(_level) << "]"
               << "[" << _pid << "]"
               << "[" << filename << "]"
               << "[" << line << "]"
               << " - ";
            _loginfo = ss.str();
        }
        template <typename T>
        LogMessage& operator << (const T &info)
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
        std::string _loginfo;   //一条合并好的，完整的日志信息
        Logger &_logger;
    };
    LogMessage operator()(LogLevel level, std::string filename, int line)
    {
        return LogMessage(level,filename,line,*this);
    }
    void EnableConsoleLogStrategy()
    {
        _strategy=std::make_unique<ConsoleLogStrategy>();
    }
    void EnableFileLogStrategy()
    {
        _strategy=std::make_unique<FileLogStrategy>();
    }
};

Logger logger;

#define LOG(level) logger(level,__FILE__,__LINE__)
#define EnableConsoleLogStrategy() logger.EnableConsoleLogStrategy()
#define EnableFileLogStrategy() logger.EnableFileLogStrategy()
