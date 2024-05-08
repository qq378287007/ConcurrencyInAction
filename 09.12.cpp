#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <thread>
using namespace std;

class interrupt_flag
{
    atomic<bool> flag;
    condition_variable *thread_cond;
    condition_variable_any *thread_cond_any;
    mutex set_clear_mutex;

public:
    interrupt_flag() : thread_cond(0), thread_cond_any(0)
    {
    }
    void set()
    {
        flag.store(true, memory_order_relaxed);
        lock_guard<mutex> lk(set_clear_mutex);
        if (thread_cond)
        {
            thread_cond->notify_all();
        }
        else if (thread_cond_any)
        {
            thread_cond_any->notify_all();
        }
    }

    template <typename Lockable>
    void wait(condition_variable_any &cv, Lockable &lk)
    {
        struct custom_lock
        {
            interrupt_flag *self;
            Lockable &lk;

            custom_lock(interrupt_flag *self_,
                        condition_variable_any &cond,
                        Lockable &lk_) : self(self_), lk(lk_)
            {
                self->set_clear_mutex.lock();
                self->thread_cond_any = &cond;
            }

            void unlock()
            {
                lk.unlock();
                self->set_clear_mutex.unlock();
            }

            void lock()
            {
                lock(self->set_clear_mutex, lk);
            }

            ~custom_lock()
            {
                self->thread_cond_any = 0;
                //self->set_clear_mutex.unlock();
            }
        };

        custom_lock cl(this, cv, lk);
        interruption_point();
        cv.wait(cl);
        interruption_point();
    }

    // rest as before
};

template <typename Lockable>
void interruptible_wait(condition_variable_any &cv, Lockable &lk)
{
    interrupt_flag this_thread_interrupt_flag;
    this_thread_interrupt_flag.wait(cv, lk);
}
