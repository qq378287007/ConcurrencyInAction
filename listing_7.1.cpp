#include <atomic>
using namespace std;

class spinlock_mutex
{
    atomic_flag flag;

public:
    spinlock_mutex() : flag(ATOMIC_FLAG_INIT) {}
    void lock()
    {
        while (flag.test_and_set(memory_order_acquire))
            ;
    }
    void unlock()
    {
        flag.clear(memory_order_release);
    }
};

int main()
{
    spinlock_mutex s;
    s.lock();
    s.unlock();
    
    return 0;
}
