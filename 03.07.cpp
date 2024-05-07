#include <mutex>
using namespace std;

class hierarchical_mutex
{
public:
    explicit hierarchical_mutex(unsigned level) {}
    void lock() {}
    void unlock() {}
};

hierarchical_mutex high_level_mutex(10000);
hierarchical_mutex low_level_mutex(5000);
hierarchical_mutex other_mutex(100);

int do_low_level_stuff() { return 42; }
int low_level_func()
{
    lock_guard<hierarchical_mutex> lk(low_level_mutex);
    return do_low_level_stuff();
}

void high_level_stuff(int some_param) {}
void high_level_func()
{
    lock_guard<hierarchical_mutex> lk(high_level_mutex);
    high_level_stuff(low_level_func());
}

// 锁住高层high_level_mutex，后锁住低层low_level_mutex
// 先调用低层函数low_level_func，后调用高层函数high_level_stuff(int)
void thread_a() { high_level_func(); }

void do_other_stuff() {}
void other_stuff()
{
    high_level_func();
    do_other_stuff();
}

// 无视规则，先锁住最低层other_mutex
void thread_b()
{
    lock_guard<hierarchical_mutex> lk(other_mutex);
    other_stuff();
}

int main()
{
    thread_a(); // ok，先高后低
    thread_b(); // error，先低后高

    return 0;
}
