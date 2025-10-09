#include <iostream>
#include <unistd.h>
#include <pthread.h>

class Task
{
public:
    Task()
    :_x(0),_y(0),_result(0),_code(0)
    {}
    Task(int x,int y)
    :_x(x),_y(y),_result(0),_code(0)
    {}
    void Div()
    {
        if(_y==0)
        {
            _code=1;    //表示除零
            return;
        }
        _result=_x/_y;
    }
    void Print()
    {
        printf("result:%d[%d]\n",_result,_code);
    }
private:
    int _x;
    int _y;
    int _result;
    int _code;
};

void *threadroutine(void *args)
{
    Task *t=static_cast<Task*>(args); 
    t->Div();
    sleep(1);
    return t;
}

int main()
{
    pthread_t tid;
    Task *t=new Task(10,0);
    pthread_create(&tid, nullptr, threadroutine, t);
    void *ret = nullptr;
    pthread_join(tid, &ret);
    Task* result=(Task*)ret;
    result->Print();
    return 0;
}