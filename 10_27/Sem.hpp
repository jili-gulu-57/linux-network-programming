#include <semaphore.h>

class Sem
{
private:
    sem_t _sem;
    int _initnum;
public:
    Sem(int num):_initnum(num)
    {
        sem_init(&_sem,0,_initnum);
    }
    ~Sem()
    {
        sem_destroy(&_sem);
    }
    void P()
    {
        int n=sem_wait(&_sem);
        (void)n;
    }
    void V()
    {
        int n=sem_post(&_sem);
        (void)n;
    }
};