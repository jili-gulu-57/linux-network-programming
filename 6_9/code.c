#include<stdio.h>
#include<unistd.h>

int main()
{
    int val=100;
    pid_t id =fork();
    if(id==0)
    {
        //child
        while(1)
        {
            printf("子进程:%d,%p\n",val,&val);
            val++;
            sleep(2);
        }
    }
    else 
    {
        //parent
        while(1)
        {
            printf("父进程:%d,%p\n",val,&val);
            sleep(2);
        }
    }
    return 0;
}
