#include<iostream>
#include<unistd.h>

int main()
{
    while(true)
    {
        std::cout<<"我是进程："<<getpid()<<std::endl;
        sleep(1);
    }
    return 0;
}
