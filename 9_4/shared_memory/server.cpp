#include "Shm.hpp"

int main()
{
    SharedMemory shm;
    shm.Creat();
    sleep(1);

    shm.Attach();
    sleep(1);

    shm.SetZero();

    char ch = 'A';
    while (true)
    {
        shm.AddChar(ch);
        sleep(3);
        ch++;
    }

    shm.Detach();
    sleep(1);

    shm.RemoveShm();
    sleep(1);
    return 0;
}