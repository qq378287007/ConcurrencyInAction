#include <memory>
#include <mutex>
#include <condition_variable>
using namespace std;

template <typename T>
class threadsafe_queue
{
private:
    struct node
    {
        shared_ptr<T> data;
        unique_ptr<node> next;
    };

    unique_ptr<node> head;
    node *tail;

    mutex head_mutex;
    mutex tail_mutex;

    condition_variable data_cond;

public:
    threadsafe_queue() : head(new node), tail(head.get()) {}
    threadsafe_queue(const threadsafe_queue &other) = delete;
    threadsafe_queue &operator=(const threadsafe_queue &other) = delete;

    shared_ptr<T> try_pop();
    bool try_pop(T &value);
    shared_ptr<T> wait_and_pop();
    void wait_and_pop(T &value);
    void push(T new_value);
    bool empty();
};

int main()
{
    return 0;
}
