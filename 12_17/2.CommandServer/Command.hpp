#pragma once

#include <iostream>
#include <cstdio>

class Command
{
public:
    Command()
    {
    }

    std::string Exec(const std::string& cmd)
    {
        FILE*fp=popen(cmd.c_str(),"r");
        if(fp==nullptr)
        {
            return cmd+" exec error";
        }
        else
    }

    ~Command()
    {}
private:
};