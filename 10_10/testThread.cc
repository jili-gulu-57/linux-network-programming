#include<iostream>
#include<thread>
#include<string>
#include<unistd.h>

//线程自己的“全局变量”
__thread int gval=0;

void Print()
{
    while(true)
    {
        printf("new thread\n");
    }
}


int main()
{
    std::thread t(Print);
    sleep(3);
    return 0;
}