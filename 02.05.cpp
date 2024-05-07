#include <thread>
#include <iostream>
using namespace std;

void some_function() {}
thread f() { return thread(some_function); }

void some_other_function(int) {}
thread g()
{
    thread t(some_other_function, 42);
    return t;
}

void f2(thread t)
{
    if (t.joinable())
        t.join();
}

void g2()
{
    f2(thread(some_function));

    thread t(some_function);
    f2(move(t));

    thread t2;
    t2 = g();
    //f2(t2);
    f2(move(t2));

    thread t3{g()};
    t3.join();

    cout << "g2 over!\n";
}

int main()
{
    thread t1 = f();
    t1.join();

    g2();

    thread t2 = g();
    t2.join();

    return 0;
}
