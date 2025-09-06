#include<iostream>

int main()
{
    int a=10;
    int *b=&a;
    std::cout<<"a:"<<sizeof(a)<<",b:"<<sizeof(b)<<std::endl;
    return 0;
}