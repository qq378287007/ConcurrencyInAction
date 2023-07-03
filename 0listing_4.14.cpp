#include <future>
using namespace std;

/*
template <typename F, typename A>
future<result_of<F(A &&)>::type> spawn_task(F &&f, A &&a)
{
    typedef result_of<F(A &&)>::type result_type;
    packaged_task<result_type(A &&)> task(move(f));
    future<result_type> res(task.get_future());
    thread t(move(task), move(a));
    t.detach();
    return res;
}
*/

template <typename F, typename... Args>
auto spawn_task(F &&f, Args &&...args) -> future<decltype(f(args...))>
{
    // Create a function with bounded parameter ready to execute
    function<decltype(f(args...))()> func = bind(forward<F>(f), forward<Args>(args)...);
    // Encapsulate it into a shared pointer in order to be able to copy construct
    auto task = make_shared<packaged_task<decltype(f(args...))()>>(func);
    // Warp packaged task into void function
    function<void()> warpper_func = [task]()
    { (*task)(); };

    // 返回先前注册的任务指针
    return task->get_future();
}

int main()
{
    return 0;
}
