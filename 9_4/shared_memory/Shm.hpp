#ifndef __SHM_HPP__
#define __SHM_HPP__

#include <iostream>
#include <sys/ipc.h>
#include <string>
#include <cstdio>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/types.h>

std::string gpathname = ".";
int gproj_id = 0x66;
int gdefaultsize = 4096;

// 共享内存
/*  1.生成key值：使用ftok()函数
    2.获取shmid值：使用shmget()函数
    3.附加到进程地址空间：使用shmat()，将共享内存映射到虚拟地址空间，获得一个可操作的指针
    4.操作：通过指针对其进行读写
    5.分离：解除共享内存和虚拟地址空间的映射关系
    6.控制：执行删除等操作
*/

struct data
{
    int num; // 使用共享内存记录数据个数
    char buffer[4092];
};

class SharedMemory
{
public:
    SharedMemory(int size = gdefaultsize)
        : _size(size), _key(0), _shmid(-1), _start_addr(nullptr),
          _windex(0), _rindex(0), _datastart(nullptr), _num(nullptr)
    {
    }
    ~SharedMemory()
    {
    }

    bool Creat()
    {
        return CreatHelper(IPC_CREAT | IPC_EXCL | 0666);
    }

    bool Get()
    {
        return CreatHelper(IPC_CREAT);
    }

    bool Attach()
    {
        _start_addr = shmat(_shmid, nullptr, 0);
        if ((long long)_start_addr == -1)
        {
            perror("shmat");
            return false;
        }
        std::cout << "映射成功" << std::endl;
        _num = (int *)_start_addr;
        _datastart = (char *)_start_addr + sizeof(int);

        return true;
    }

    void SetZero()
    {
        *_num = 0;
        return;
    }

    void AddChar(char ch)
    {
        if (*_num == _size)
            return;
        ((char *)_datastart)[_windex++] = ch;
        ((char *)_datastart)[_windex] ='\0';
        _windex %= _size;
        (*_num)++;
        std::cout<<"write:"<<ch<<std::endl;
    }

    void PopChar(char *ch)
    {
        if (*_num == 0)
            return;
        *ch = ((char *)_datastart)[_rindex++];
        _rindex %= _size;
        (*_num)--;
        std::cout<<"read:"<<ch<<std::endl;
    }

    bool Detach()
    {
        int n = shmdt(_start_addr);
        if (n < 0)
        {
            perror("shmdt");
            return false;
        }
        std::cout << "解除映射成功" << std::endl;
        return true;
    }

    bool RemoveShm()
    {
        int n = shmctl(_shmid, IPC_RMID, nullptr); // 成功返回0，失败返回-1
        if (n < 0)
        {
            perror("shmctl");
            return false;
        }
        std::cout << "删除shm成功" << std::endl;
        return true;
    }

private:
    key_t _key;
    int _size; // 内存块大小
    int _shmid;
    void *_start_addr; // 虚拟地址起始地址
    int _windex;       // 写操作位置下标
    int _rindex;       // 读操作位置下标
    char *_datastart;  // 共享内存开始记录数据的位置
    int *_num;

    bool CreatHelper(int flags)
    {
        _key = ftok(gpathname.c_str(), gproj_id);
        if (_key < 0)
        {
            perror("ftok");
            return false;
        }
        printf("形成key成功：0x%x\n", _key);

        _shmid = shmget(_key, _size, flags);
        if (_shmid < 0)
        {
            perror("shmget");
            return false;
        }
        printf("形成shmid成功：%d\n", _shmid);
        return true;
    }
};

#endif