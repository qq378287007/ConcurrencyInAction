#include <string>
#include <thread>
#include <atomic>
#include <cassert>
using namespace std;

struct X
{
    int i;
    string s;
};

atomic<X *> p;
atomic<int> a;

void create_x()
{
    X *x = new X;
    x->i = 42;
    x->s = "hello";
    a.store(99, memory_order_relaxed);
    p.store(x, memory_order_release);
}

void use_x()
{
    X *x;
    while (!(x = p.load(memory_order_consume)))
        this_thread::sleep_for(chrono::microseconds(1));
    assert(x->i == 42);
    assert(x->s == "hello");
    assert(a.load(memory_order_relaxed) == 99);
}

int main()
{
    thread t1(create_x);
    thread t2(use_x);
    t1.join();
    t2.join();
    return 0;
}
