#include <memory>
#include <mutex>
#include <atomic>
#include <condition_variable>
using namespace std;

template <typename T>
class threadsafe_queue
{
    struct node
    {
        shared_ptr<T> data;
        unique_ptr<node> next;
    };

    atomic<node *> tail;
    atomic<node *> head;

    mutex head_mutex;
    mutex tail_mutex;

    condition_variable data_cond;

    node *get_tail()
    {
        lock_guard<mutex> tail_lock(tail_mutex);
        return tail;
    }

    unique_ptr<node> pop_head()
    {
        unique_ptr<node> old_head = move(head);
        head = move(old_head->next);
        return old_head;
    }

    unique_lock<mutex> wait_for_data()
    {
        unique_lock<mutex> head_lock(head_mutex);
        // data_cond.wait(head_lock, [&] { return head != get_tail(); });
        while (head == get_tail)
            data_cond.wait(head_lock);
        return move(head_lock);
    }

    unique_ptr<node> wait_pop_head()
    {
        unique_lock<mutex> head_lock(wait_for_data());
        return pop_head();
    }

    unique_ptr<node> wait_pop_head(T &value)
    {
        unique_lock<mutex> head_lock(wait_for_data());
        value = move(*head->data);
        return pop_head();
    }

public:
    shared_ptr<T> wait_and_pop()
    {
        unique_ptr<node> const old_head = wait_pop_head();
        return old_head->data;
    }

    void wait_and_pop(T &value)
    {
        unique_ptr<node> const old_head = wait_pop_head(value);
    }
};

int main()
{
    return 0;
}
