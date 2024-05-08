#include <memory>
#include <mutex>
using namespace std;

template <typename T>
class threadsafe_queue
{
    struct node
    {
        shared_ptr<T> data;
        unique_ptr<node> next;
    };

    unique_ptr<node> head;
    node *tail;

    mutex head_mutex;
    mutex tail_mutex;

    node *get_tail()
    {
        lock_guard<mutex> tail_lock(tail_mutex);
        return tail;
    }

    unique_ptr<node> pop_head()
    {
        lock_guard<mutex> head_lock(head_mutex);
        if (head.get() == get_tail())
            return nullptr;

        unique_ptr<node> old_head = move(head);
        head = move(old_head->next);
        return old_head;
    }

    unique_ptr<node> try_pop_head()
    {
        lock_guard<mutex> head_lock(head_mutex);
        if (head.get() == get_tail())
            // return unique_ptr<node>();
            return nullptr;

        return pop_head();
    }

    unique_ptr<node> try_pop_head(T &value)
    {
        lock_guard<mutex> head_lock(head_mutex);
        if (head.get() == get_tail())
            // return unique_ptr<node>();
            return nullptr;

        value = move(*head->data);
        return pop_head();
    }

public:
    threadsafe_queue() : head(new node), tail(head.get()) {}
    threadsafe_queue(const threadsafe_queue &other) = delete;
    threadsafe_queue &operator=(const threadsafe_queue &other) = delete;

public:
    shared_ptr<T> try_pop()
    {
        unique_ptr<node> const old_head = try_pop_head();
        return old_head ? old_head->data : shared_ptr<T>();
    }

    bool try_pop(T &value)
    {
        unique_ptr<node> const old_head = try_pop_head(value);
        return old_head;
    }

    bool empty()
    {
        lock_guard<mutex> head_lock(head_mutex);
        return head == get_tail();
    }
};

int main()
{
    return 0;
}
