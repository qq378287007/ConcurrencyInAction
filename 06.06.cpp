#include <memory>
#include <mutex>
#include <iostream>
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

public:
    threadsafe_queue() : head(new node), tail(head.get()) {}

    threadsafe_queue(const threadsafe_queue &other) = delete;
    threadsafe_queue &operator=(const threadsafe_queue &other) = delete;

    shared_ptr<T> try_pop()
    {
        unique_ptr<node> old_head = pop_head();
        return old_head ? old_head->data : shared_ptr<T>();
    }

    void push(T new_value)
    {
        shared_ptr<T> new_data(make_shared<T>(move(new_value)));
        unique_ptr<node> p(new node);
        node *const new_tail = p.get();

        lock_guard<mutex> tail_lock(tail_mutex);
        tail->data = new_data;
        tail->next = move(p);
        tail = new_tail;
    }
};

int main()
{
    threadsafe_queue<int> q;
    q.push(1);
    q.push(2);

    auto d1 = q.try_pop();
    cout << *d1 << endl;

    auto d2 = q.try_pop();
    cout << *d2 << endl;

    cout << boolalpha << (q.try_pop() == nullptr) << endl;

    return 0;
}
