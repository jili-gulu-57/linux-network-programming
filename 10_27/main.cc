#include "RingQueue.hpp"

void *consumer(void *args)
{
    RingQueue<int> *rq = static_cast<RingQueue<int> *>(args);
    int data = 0;
    while (true)
    {
        rq->Pop(&data);
        std::cout << "消费者消费了一个数据：" << data << std::endl;
    }
}

void *productor(void *args)
{
    RingQueue<int> *rq = static_cast<RingQueue<int> *>(args);
    int data = 1;
    while (true)
    {
        rq->Enqueue(data);
        std::cout << "生产者生产了一个数据：" << data++ << std::endl;
    }
}

int main()
{
    RingQueue<int> *rq = new RingQueue<int>();
    pthread_t c, p;
    pthread_create(&c, nullptr, consumer, (void *)rq);
    pthread_create(&p, nullptr, productor, (void *)rq);

    pthread_join(c,nullptr);
    pthread_join(p,nullptr);
    delete rq;
    return 0;
}