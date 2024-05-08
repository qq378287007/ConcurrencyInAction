#include <deque>
#include <mutex>
#include <memory>
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

class work_stealing_queue
{
private:
    typedef function_wrapper data_type;
    deque<data_type> the_queue;
    mutable mutex the_mutex;

public:
    work_stealing_queue()
    {
    }

    work_stealing_queue(const work_stealing_queue &other) = delete;
    work_stealing_queue &operator=(
        const work_stealing_queue &other) = delete;

    void push(data_type data)
    {
        lock_guard<mutex> lock(the_mutex);
        the_queue.push_front(move(data));
    }

    bool empty() const
    {
        lock_guard<mutex> lock(the_mutex);
        return the_queue.empty();
    }

    bool try_pop(data_type &res)
    {
        lock_guard<mutex> lock(the_mutex);
        if (the_queue.empty())
        {
            return false;
        }

        res = move(the_queue.front());
        the_queue.pop_front();
        return true;
    }

    bool try_steal(data_type &res)
    {
        lock_guard<mutex> lock(the_mutex);
        if (the_queue.empty())
        {
            return false;
        }

        res = move(the_queue.back());
        the_queue.pop_back();
        return true;
    }
};
