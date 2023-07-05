#include <memory>
#include <mutex>
using namespace std;

struct some_resource
{
    void do_something() {}
};

shared_ptr<some_resource> resource_ptr = nullptr;
mutex resource_mutex;

void foo()
{
    resource_mutex.lock();
    if (!resource_ptr)
        resource_ptr.reset(new some_resource);
    resource_mutex.unlock();

    // unique_lock<mutex> lk(resource_mutex);
    // if (!resource_ptr)
    //     resource_ptr.reset(new some_resource);
    // lk.unlock();

    resource_ptr->do_something();
}

// 双重检验锁定模式（double-checked locking pattern）
void undefined_behaviour_with_double_checked_locking()
{
    if (!resource_ptr)
    {
        std::lock_guard<std::mutex> lk(resource_mutex);
        if (!resource_ptr)
            resource_ptr.reset(new some_resource);
    }
    resource_ptr->do_something();
}

int main()
{
    foo();
    undefined_behaviour_with_double_checked_locking();
    
    return 0;
}
