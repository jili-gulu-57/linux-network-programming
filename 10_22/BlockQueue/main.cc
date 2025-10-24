#include "BlockQueue.hpp"
#include <unistd.h>

struct ThreadData
{
    BlockQueue<int> *bq;
    std::string name;
};

void *consumer(void *args)
{
    ThreadData *td = static_cast<ThreadData *>(args);
    int data = 0;
    while (true)
    {
        sleep(1);
        td->bq->Pop(&data);
        std::cout << "消费者消费了一个数据：" << data << std::endl;
    }
}

void *productor(void *args)
{
    ThreadData *td = static_cast<ThreadData *>(args);
    int data = 1;
    while (true)
    {
        td->bq->Enqueue(data);
        std::cout << "生产者生产了一个数据：" << data++ << std::endl;
    }
}

int main()
{
    BlockQueue<int> *bq = new BlockQueue<int>();
    pthread_t c, p;

    ThreadData ctd = {bq, "消费者"};
    pthread_create(&c, nullptr, consumer, (void *)&ctd);

    ThreadData ptd = {bq, "生产者"};
    pthread_create(&p, nullptr, productor, (void *)&ptd);

    pthread_join(c, nullptr);
    pthread_join(p, nullptr);
    return 0;
}