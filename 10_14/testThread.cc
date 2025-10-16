#include "Thread.hpp"
#include<vector>

void test()
{
    int cnt=5;
    while(cnt--)
    {
        std::cout<<"new thread is running"<<std::endl;
        sleep(1);
    }
}

int main()
{
    //单个线程
    // Thread t(test,"thread-1");
    // t.Start();
    // t.Join();

    //多个线程
    std::vector<Thread> threads;
    for(int i=0;i<5;i++)
    {
        std::string name="thread-";
        name+=std::to_string(i+1);
        Thread t(test,name);
        threads.push_back(t);
    }
    return 0;
}