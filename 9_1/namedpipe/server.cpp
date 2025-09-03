#include "NamedPipe.hpp"

int main()
{
    NamedPipe named_pipe(fifoname);
    named_pipe.Creat();
    named_pipe.OpenForRead();

    std::string message;
    while (true)
    {
        bool ret = named_pipe.Read(&message);
        if (!ret)
            break;

        std::cout<<"收到的信息为："<<message<<std::endl;
    }

    named_pipe.Close();
    named_pipe.Remove();
    return 0;
}