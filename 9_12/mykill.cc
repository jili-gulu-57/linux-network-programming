#include <iostream>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

void handler(int signo)
{
    printf("捕捉到了信号%d\n",signo);
    return;
}

int main()
{
    signal(11,handler);

    // int a=10;
    // a/=0;
    int *p=nullptr;
    *p=10;
    signal(8,handler);
    printf("进程崩溃了\n");
    return 0;
}


//  ./mykill 9 12424
// int main(int argc, char *argv[])
// {

//     alarm(3);

//     while(true)
//     {
//         std::cout<<"1"<<std::endl;
//         sleep(1);
//     }

//     // if (argc != 3)
//     // {
//     //     std::cout << "Usage:" << argv[0] << "signumber pid" << std::endl;
//     //     return 1;
//     // }
//     // int signumber = std::stoi(argv[1]);
//     // pid_t target = std::stoi(argv[2]);

//     // int n = kill(target, signumber);
//     // if (n < 0)
//     // {
//     //     perror("kill");
//     //     return 2;
//     // }

//     // signal(6,handler);

//     // while(true)
//     // {
//     //     abort();   //程序自己给自己发信号
//     //     sleep(1);
//     // }

//     return 0;
// }