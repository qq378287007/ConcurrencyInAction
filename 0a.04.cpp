#include <condition_variable>
#include <mutex>
using namespace std;

condition_variable cond;
bool data_ready;
mutex m;

void wait_for_data()
{
    unique_lock<mutex> lk(m);
    // cond.wait(lk, [] { return data_ready; });
    while (data_ready)
        cond.wait(lk);
}

int main()
{
    return 0;
}
