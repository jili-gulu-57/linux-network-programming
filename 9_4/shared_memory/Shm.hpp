#ifndef __SHM_HPP__
#define __SHM_HPP__

#include<iostream>
#include<sys/ipc.h>
#include<string>
#include<cstdio>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/types.h>

std::string gpathname=".";
int gproj_id=0x66;
int gdefaultsize=4096;

//共享内存
/*  1.生成key值
    2.获取shmid值
    3.
*/
class SharedMemory
{
public:
    SharedMemory(int size=gdefaultsize)
    :_size(size) 
    ,_key(0)
    ,_shmid(-1)
    {}
    ~SharedMemory()
    {}

    bool Creat()
    {
        _key=ftok(gpathname.c_str(),gproj_id);
        if(_key<0)
        {
            perror("ftok");
            return false;
        }
        printf("形成key成功：0x%x\n",_key);

        _shmid=shmget(_key,_size,IPC_CREAT|IPC_EXCL);   //创建成功返回shmid，失败返回-1
        if(_shmid<0)
        {
            perror("shmget");
            return false;
        }
        printf("形成shmid成功：%d\n",_shmid);
        return true;
    }

private:
    key_t _key;
    int _size;  //内存块大小
    int _shmid;
};

#endif