#include <future>
#include <algorithm>
using namespace std;

void interruption_point() {};
class interrupt_flag
{
    atomic<bool> flag;
    condition_variable *thread_cond;
    mutex set_clear_mutex;

public:
    interrupt_flag() : thread_cond(0)
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
    }
    bool is_set() const
    {
        return flag.load(memory_order_relaxed);
    }

    void set_condition_variable(condition_variable &cv)
    {
        lock_guard<mutex> lk(set_clear_mutex);
        thread_cond = &cv;
    }

    void clear_condition_variable()
    {
        lock_guard<mutex> lk(set_clear_mutex);
        thread_cond = 0;
    }

    struct clear_cv_on_destruct
    {
        ~clear_cv_on_destruct()
        {
            this_thread_interrupt_flag.clear_condition_variable();
        }
    };
};
static interrupt_flag this_thread_interrupt_flag;

void interruptible_wait(condition_variable &cv, unique_lock<mutex> &lk)
{
    interruption_point();
    interrupt_flag this_thread_interrupt_flag;
    //    this_thread_interrupt_flag.set_condition_variable(cv);
    interrupt_flag::clear_cv_on_destruct guard;
    interruption_point();
    cv.wait_for(lk, chrono::milliseconds(1));
    interruption_point();
}
