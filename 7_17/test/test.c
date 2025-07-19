#include <stdio.h>
#include <string.h>

int main()
{
    char buffer[]="ls -a -b -c";
    char *substr=strtok(buffer," ");
    printf("%s\n",substr);

    return 0;
}
