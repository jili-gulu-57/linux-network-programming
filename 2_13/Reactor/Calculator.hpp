#pragma once

#include <iostream>
#include <string>
#include "Parser.hpp"
#include "Protocol.hpp"

class Calculator
{
public:
    Calculator()
    {
    }

    ResPonse Exec(Request &req)
    {
        ResPonse resp;
        switch (req.Oper())
        {
        case '+':
            resp.Result(req.X() + req.Y());
            break;
        case '-':
            resp.Result(req.X() - req.Y());
            break;
        case '*':
            resp.Result(req.X() * req.Y());
            break;
        case '/':
            if (req.Y() == 0)
            {
                resp.Code(1); // 1 : 除0错误
            }
            else
            {
                resp.Result(req.X() + req.Y());
            }
            break;

        default:
            resp.Code(2);
            std::cout << "不支持该运算" << std::endl;
            break;
        }
        return resp;
    }

    ~Calculator()
    {
    }
};