#include <deque>
#include <future>
#include <memory>
#include <functional>
#include <iostream>
#include <iostream>
using namespace std;

class function_wrapper
{
    struct impl_base
    {
        virtual ~impl_base() {}
        virtual void call() = 0;
    };
    unique_ptr<impl_base> impl;
    template <typename F>
    struct impl_type : impl_base
    {
        F f;
        impl_type(F &&f_) : f(move(f_)) {}
        void call() { f(); }
    };

public:
    template <typename F>
    function_wrapper(F &&f) : impl(new impl_type<F>(move(f)))
    {
    }

    void call() { impl->call(); }

    function_wrapper(function_wrapper &&other) : impl(move(other.impl))
    {
    }

    function_wrapper &operator=(function_wrapper &&other)
    {
        impl = move(other.impl);
        return *this;
    }

    function_wrapper(const function_wrapper &) = delete;
    function_wrapper(function_wrapper &) = delete;
    function_wrapper &operator=(const function_wrapper &) = delete;
};

class thread_pool
{
public:
    deque<function_wrapper> work_queue;

    template <typename FunctionType>
    future<typename result_of<FunctionType()>::type>
    submit(FunctionType f)
    {
        typedef typename result_of<FunctionType()>::type result_type;

        packaged_task<result_type()> task(move(f));
        future<result_type> res(task.get_future());
        work_queue.push_back(move(task));
        return res;
    }
    // rest as before
};
