#include"ThreadPool.hpp"
#include<memory>

int main()
{
    std::unique_ptr<ThreadPool> tp=std::make_unique<ThreadPool>();
    
    return 0;
}
