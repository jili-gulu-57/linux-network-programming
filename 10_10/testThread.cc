#include<iostream>
#include<thread>
#include<string>
#include<unistd.h>

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