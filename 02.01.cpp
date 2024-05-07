#include <thread>
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

void oops()
{
    int some_local_state = 0;
    func my_func(some_local_state);
    thread my_thread(my_func);
    // my_thread.detach();
    my_thread.join();

    cout << "some_local_state: " << some_local_state << endl;
}

int main()
{
    oops();
    return 0;
}
