#include <vector>
#include <thread>
#include <algorithm>
#include <functional>
using namespace std;

void do_work(unsigned id)
{
}

void f()
{
    vector<thread> threads;
    for (unsigned i = 0; i < 20; ++i)
        threads.push_back(thread(do_work, i));

    for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
}

int main()
{
    f();
    return 0;
}
