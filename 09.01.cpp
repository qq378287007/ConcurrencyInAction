#include <thread>
#include <vector>
#include <atomic>
using namespace std;

struct join_threads
{
    join_threads(vector<thread> &) {}
};

class thread_pool
{
    atomic_bool done;
    thread_safe_queue<function<void()>> work_queue;
    vector<thread> threads;
    join_threads joiner;

    void worker_thread()
    {
        while (!done)
        {
            function<void()> task;
            if (work_queue.try_pop(task))
            {
                task();
            }
            else
            {
                this_thread::yield();
            }
        }
    }

public:
    thread_pool() : done(false), joiner(threads)
    {
        unsigned const thread_count = thread::hardware_concurrency();
        try
        {
            for (unsigned i = 0; i < thread_count; ++i)
            {
                threads.push_back(
                    thread(&thread_pool::worker_thread, this));
            }
        }
        catch (...)
        {
            done = true;
            throw;
        }
    }

    ~thread_pool()
    {
        done = true;
    }

    template <typename FunctionType>
    void submit(FunctionType f)
    {
        work_queue.push(function<void()>(f));
    }
};
