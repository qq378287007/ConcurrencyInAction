#include <memory>
#include <mutex>
using namespace std;

struct some_resource
{
    void do_something()
    {
    }
};

shared_ptr<some_resource> resource_ptr;
mutex resource_mutex;

void foo()
{
    unique_lock<mutex> lk(resource_mutex);
    if (!resource_ptr)
        resource_ptr.reset(new some_resource);
    lk.unlock();
    
    resource_ptr->do_something();
}

int main()
{
    foo();
    return 0;
}
