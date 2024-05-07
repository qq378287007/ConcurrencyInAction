#include <experimental/future>
#include <thread>
#include <iostream>
using namespace std;

// decltype，表达式类型
// declval，得到T&&虚假实例，不需要构造函数
// decay_t，返回T类型
template <typename Func>
experimental::future<decltype(declval<Func>()())> spawn_async(Func &&func)
{
    experimental::promise<decltype(declval<Func>()())> p;
    auto res = p.get_future();
    thread t(
        [p = move(p), f = decay_t<Func>(func)]() mutable
        {
            try
            {
                // 原子地存储 value 到共享状态，而不立即令状态就绪。
                // 在当前线程退出时，销毁所有拥有线程局域存储期的对象后，再令状态就绪。
                //p.set_value_at_thread_exit(f());
                p.set_value(f());
            }
            catch (...)
            {
                //p.set_exception_at_thread_exit(current_exception());
                p.set_exception(current_exception());
            }
        });
    t.detach();
    return res;
}

int main()
{
    auto ans = spawn_async([]
                           { return 5; });
    cout << ans.get() << endl;
    return 0;
}

// g++ listing_4.17.cpp -o listing_4.17 -I. -Iexperimental/deail && listing_4.17
