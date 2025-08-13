#include "mystdio.h"
#include <string.h>
#include <unistd.h>

int main()
{
    MyFILE *fp=my_fopen("log.txt","w");
    if(fp==NULL)
        return 1;
    const char *s="hello,myfile\n";
    int cnt=3;
    while(cnt--)
    {
        my_fwrite(s,strlen(s),fp);
        sleep(1);
    }

    my_fclose(fp);
    return 0;
}

