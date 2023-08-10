#include <future>
#include <thread>
#include <functional>
#include <iostream>
using namespace std;

/*
template <typename F, typename A>
future<typename result_of<F(A &&)>::type> spawn_task1(F &&f, A &&a)
{
    typedef typename result_of<F(A &&)>::type result_type;
    packaged_task<result_type(A &&)> task(move(f));
    future<result_type> res(task.get_future());
    thread t(move(task), move(a));
    t.detach();
    return res;
}
*/

template <typename F, typename... A>
future<typename result_of<F(A &&...)>::type> spawn_task1(F &&f, A &&...a)
{
    typedef typename result_of<F(A && ...)>::type result_type;
    packaged_task<result_type(A && ...)> task(move(f));
    future<result_type> res(task.get_future());
    // thread t(move(task), move(a)...);
    thread t(move(task), forward<A>(a)...);
    t.detach();
    return res;
}
template <typename F, typename... Args>
auto spawn_task2(F &&f, Args &&...args) -> future<decltype(f(args...))>
{
    // Create a function with bounded parameter ready to execute
    function<decltype(f(args...))()> func = bind(forward<F>(f), forward<Args>(args)...);

    // Encapsulate it into a shared pointer in order to be able to copy construct
    auto task = make_shared<packaged_task<decltype(f(args...))()>>(func);

    // Warp packaged task into void function
    function<void()> warpper_func = [task]()
    { (*task)(); }; // 可以传递给线程队列

    warpper_func();

    // 返回先前注册的任务指针
    return task->get_future();
}

int main()
{
    auto ans1 = spawn_task1([](int)
                            { return 5; },
                            1);
    cout << ans1.get() << endl;

    auto ans11 = spawn_task1([](int, int)
                             { return 15; },
                             1, 2);
    cout << ans11.get() << endl;

    auto ans2 = spawn_task2([](int)
                            { return 5; },
                            1);
    cout << ans2.get() << endl;

    auto ans3 = spawn_task2([](int, int)
                            { return 25; },
                            1, 2);
    cout << ans3.get() << endl;

    return 0;
}
