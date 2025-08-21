#include<iostream>
#include<cstdio>
#include<unistd.h>

int main()
{
    int pipefd[2]={0};
    int n=pipe(pipefd);
    if(n==0)
    {
        // std::cout<<"creat pipe success!"<<std::endl;
        printf("creat pipe success,pipefd[0]:%d,pipefd[1]:%d\n",pipefd[0],pipefd[1]);
    }
    return 0;
}