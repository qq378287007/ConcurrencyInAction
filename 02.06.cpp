#include <thread>
#include <utility>
#include <stdexcept>
#include <iostream>
using namespace std;

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

class scoped_thread
{
    thread t;

public:
    explicit scoped_thread(thread t_) : t(move(t_))
    {
        cout << "scoped_thread" << endl;
        if (!t.joinable())
            throw logic_error("No thread");
    }
    ~scoped_thread()
    {
        cout << "~scoped_thread" << endl;
        if (t.joinable())
            t.join();
    }
    scoped_thread(scoped_thread const &) = delete;
    scoped_thread &operator=(scoped_thread const &) = delete;
};

void do_something_in_current_thread() {}

void f(int &some_local_state)
{
    // scoped_thread t(thread(func(some_local_state)));
    scoped_thread t(thread{func(some_local_state)});
    do_something_in_current_thread();
}

void f2(int &some_local_state)
{
    scoped_thread t{thread(func(some_local_state))};
    do_something_in_current_thread();
}

int main()
{
    int some_local_state = 0;
    f(some_local_state);
    f2(some_local_state);
    cout << "some_local_state: " << some_local_state << endl;
    return 0;
}
