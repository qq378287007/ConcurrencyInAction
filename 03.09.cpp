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

        // adopt_lock表明mutex已调用lock()，unique_lock构造函数不再调用lock()
        // defer_lock表明unique_lock构造函数不调用lock()，后面会调用
        unique_lock<mutex> lock_a(lhs.m, defer_lock);
        unique_lock<mutex> lock_b(rhs.m, defer_lock);
        lock(lock_a, lock_b);
        swap(lhs.some_detail, rhs.some_detail);
    }
};

int main()
{
    X x1;
    X x2;
    swap(x1, x2);
    return 0;
}
