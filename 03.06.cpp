#include <mutex>
using namespace std;

class some_big_object{};
void swap(some_big_object &lhs, some_big_object &rhs) {}

class X
{
private:
    some_big_object some_detail;
    mutable mutex m;

public:
    X() = default;
    X(some_big_object const &sd) : some_detail(sd) {}

    friend void swap(X &lhs, X &rhs)
    {
        if (&lhs == &rhs)
            return;

        // c++11
        lock(lhs.m, rhs.m);
        lock_guard<mutex> lock_a(lhs.m, adopt_lock); // adopt_lock指明互斥已被锁住
        lock_guard<mutex> lock_b(rhs.m, adopt_lock);

        // c++17
        // scoped_lock guard(lhs.m, rhs.m);
        // scoped_lock<mutex, mutex> guard(lhs.m, rhs.m);
        
        swap(lhs.some_detail, rhs.some_detail);
    }
};

int main()
{
    X lhs;
    X rhs;
    swap(lhs, rhs);
    return 0;
}
