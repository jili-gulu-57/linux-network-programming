#include <iostream>
#include <fcntl.h>
#include<unistd.h>

void SetNonBlock(int sockfd)
{
    int f1 = fcntl(sockfd, F_GETFL);
    if(f1<0)
    {
        perror("fcntl");
        return;
    }
    fcntl(sockfd, F_SETFL, f1 | O_NONBLOCK);
}

int main()
{
    char buffer[64];
    while (true)
    {
        ssize_t n = read(0, buffer, sizeof(buffer) - 1);
        if(n>0)
        {
            buffer[n] = 0;
            printf("#%s", buffer);
        }
        else if(n==0)
        {
            printf("read end of file!");
        }
        else
        {
            perror("read fail");
        }
    }

    return 0;
}