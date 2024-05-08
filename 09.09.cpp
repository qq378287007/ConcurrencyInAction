#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <queue>
using namespace std;

template <typename T>
class threadsafe_queue
{
    mutex mut;
    queue<T> data_queue;
    condition_variable data_cond;

public:
    void push(T new_value)
    {
        lock_guard<mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }

    void wait_and_pop(T &value)
    {
        unique_lock<mutex> lk(mut);
        while (data_queue.empty())
            data_cond.wait(lk);
        value = data_queue.front();
        data_queue.pop();
    }
};

class interrupt_flag
{
public:
    void set();
    bool is_set() const;
};
thread_local interrupt_flag this_thread_interrupt_flag;

class interruptible_thread
{
    thread internal_thread;
    interrupt_flag *flag;

public:
    template <typename FunctionType>
    interruptible_thread(FunctionType f)
    {
        promise<interrupt_flag *> p;
        internal_thread = thread([f, &p]
                                 {
                p.set_value(&this_thread_interrupt_flag);
                f(); });
        flag = p.get_future().get();
    }
    void interrupt()
    {
        if (flag)
        {
            flag->set();
        }
    }
};
