#include <atomic>
#include <thread>
#include <cassert>
using namespace std;

atomic<bool> x, y;
atomic<int> z;

void write_x()
{
    x.store(true, memory_order_release);
}

void write_y()
{
    y.store(true, memory_order_release);
}

void read_x_then_y()
{
    while (!x.load(memory_order_acquire))
        ;
    if (y.load(memory_order_acquire))
        ++z;
}

void read_y_then_x()
{
    while (!y.load(memory_order_acquire))
        ;
    if (x.load(memory_order_acquire))
        ++z;
}

int main()
{
    x = false;
    y = false;
    z = 0;
    thread a(write_x);
    thread b(write_y);
    thread c(read_x_then_y);
    thread d(read_y_then_x);
    a.join();
    b.join();
    c.join();
    d.join();
    assert(z.load() != 0);
    return 0;
}
