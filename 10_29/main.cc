#include"Logger.hpp"
#include<unistd.h>

int main()
{
    EnableConsoleLogStrategy();
    // EnableFileLogStrategy();
    LOG(LogLevel::DEBUG)<<"hello,world"<<1234;
    LOG(LogLevel::WARNING)<<"hello,world"<<1234;
    LOG(LogLevel::DEBUG)<<"hello,world"<<1234;
    LOG(LogLevel::DEBUG)<<"hello,world"<<1234;
    return 0;
}