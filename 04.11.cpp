#include <condition_variable>
#include <mutex>
#include <chrono>
using namespace std;

condition_variable cv;
bool done;
mutex m;
bool wait_loop()
{
    auto const timeout = chrono::steady_clock::now() + chrono::milliseconds(500);
    unique_lock<mutex> lk(m);
    while (!done)
    {
        if (cv.wait_until(lk, timeout) == cv_status::timeout)
            break;
    }
    return done;
}

int main()
{
    wait_loop();
    
    return 0;
}