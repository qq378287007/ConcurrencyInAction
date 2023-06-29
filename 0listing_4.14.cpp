#include <future>
using namespace std;

template <typename F, typename A>
future<result_of<F(A &&)>::type>
spawn_task(F &&f, A &&a)
{
    typedef result_of<F(A &&)>::type result_type;
    packaged_task<result_type(A &&)> task(move(f));
    future<result_type> res(task.get_future());
    thread t(move(task), move(a));
    t.detach();
    return res;
}

int main()
{
    return 0;
}
