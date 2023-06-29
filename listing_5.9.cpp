#include <atomic>
#include <thread>
#include <cassert>
using namespace std;

atomic<int> data[5];
atomic<bool> sync1(false), sync2(false);

void thread_1()
{
    data[0].store(42, memory_order_relaxed);
    data[1].store(97, memory_order_relaxed);
    data[2].store(17, memory_order_relaxed);
    data[3].store(-141, memory_order_relaxed);
    data[4].store(2003, memory_order_relaxed);
    sync1.store(true, memory_order_release);
}

void thread_2()
{
    while (!sync1.load(memory_order_acquire))
        ;
    sync2.store(memory_order_release);
}

void thread_3()
{
    while (!sync2.load(memory_order_acquire))
        ;
    assert(data[0].load(memory_order_relaxed) == 42);
    assert(data[1].load(memory_order_relaxed) == 97);
    assert(data[2].load(memory_order_relaxed) == 17);
    assert(data[3].load(memory_order_relaxed) == -141);
    assert(data[4].load(memory_order_relaxed) == 2003);
}

int main()
{
    thread t1(thread_1);
    thread t2(thread_2);
    thread t3(thread_3);
    t1.join();
    t2.join();
    t3.join();
    return 0;
}
