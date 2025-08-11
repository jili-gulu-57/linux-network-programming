#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
    if(argc!=2)
    {
        fprintf(stderr,"Usage:%s<directory>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    DIR* dir=opendir(argv[1]);
    if(!dir)
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while((entry=readdir(dir))!=NULL)
    {
        if(strcmp(entry->d_name,".")==0||strcmp(entry->d_name,"..")==0)
        {
            continue;
        }

        printf("Filename:%s,Inode:%lu\n",entry->d_name,(unsigned long)entry->d_ino);
    }

    closedir(dir);
    return 0;

}
