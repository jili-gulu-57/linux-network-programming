#include "InetAddr.hpp"
#include <iostream>
#include <vector>

class Route
{
private:
    bool IsExists(const InetAddr &addr)
    {
        for (auto &user : _online_user)
        {
            if (user == addr)
                return true;
        }
        return false;
    }

public:
    Route(/* args */);
    ~Route();

private:
    std::vector<InetAddr> _online_user;
};
