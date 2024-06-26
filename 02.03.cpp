#include <thread>
#include <iostream>
using namespace std;

class thread_guard
{
    thread &t;
public:
    explicit thread_guard(thread &t_) : t(t_) {}
    ~thread_guard()
    {
        if (t.joinable())
            t.join();
    }
    thread_guard(thread_guard const &) = delete;
    thread_guard &operator=(thread_guard const &) = delete;
};

void do_something(int &i) { ++i; }
struct func
{
    int &i;
    func(int &i_) : i(i_) {}

    void operator()()
    {
        for (unsigned j = 0; j < 1000000; ++j)
            do_something(i);
    }
};

void do_something_in_current_thread() {}
void f(int &some_local_state)
{
    func my_func(some_local_state);
    thread t(my_func);
    thread_guard g(t);

    do_something_in_current_thread();
}

int main()
{
    int some_local_state = 0;
    f(some_local_state);
    cout << "some_local_state: " << some_local_state << endl;
    return 0;
}
