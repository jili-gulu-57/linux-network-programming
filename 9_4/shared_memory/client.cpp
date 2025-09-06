#include "Shm.hpp"

int main()
{
    SharedMemory shm;
    shm.Get();
    sleep(1);

    shm.Attach();
    sleep(1);

    char *ch;
    int n=26;
    while(true)
    {
        shm.PopChar(ch);
    }

    shm.Detach();
    sleep(1);
    return 0;
}