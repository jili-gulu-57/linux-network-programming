#include"NamedPipe.hpp"

int main()
{
    NamedPipe named_pipe(fifoname);
    named_pipe.OpenForWrite();

    while(true)
    {
        std::string line;
        std::cout<<"please input:";
        std::getline(std::cin,line);
        named_pipe.Write(line);
    }

    named_pipe.Close();
    return 0;
}