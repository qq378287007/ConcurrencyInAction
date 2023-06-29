#include <memory>
#include <mutex>
using namespace std;

template <typename T>
class threadsafe_queue
{
private:
    unique_ptr<node> try_pop_head()
    {
        lock_guard<mutex> head_lock(head_mutex);
        if (head.get() == get_tail())
            return unique_ptr<node>();
        
        return pop_head();
    }

    unique_ptr<node> try_pop_head(T &value)
    {
        lock_guard<mutex> head_lock(head_mutex);
        if (head.get() == get_tail())
            return unique_ptr<node>();
        
        value = move(*head->data);
        return pop_head();
    }

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
        return (head == get_tail());
    }
};

int main()
{
    return 0;
}