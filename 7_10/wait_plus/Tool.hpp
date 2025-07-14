#pragma once 

#include<iostream>
#include <vector>
#include <functional>

using func_t =std::function<void()>;
//等同于typeedf std::function<void()> func_t;

class Tool
{
    public:
        Tool(){}
        void Pushfunc(func_t f)
        {
            _funcs.push_back(f);
        }
        void Execute()
        {
            for(auto &f:_funcs)
            {
                f();
            }
        }
        ~Tool(){}

    private:
        std::vector<func_t> _funcs;
};
