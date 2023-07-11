#include <memory>
#include <mutex>
#include <condition_variable>
#include <iostream>
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

    shared_ptr<T> try_pop()
    {
        lock_guard<mutex> tail_lock(head_mutex);

        if (head.get() == tail)
            return shared_ptr<T>();

        shared_ptr<T> const res(head->data);
        unique_ptr<node> const old_head = move(head);
        head = move(old_head->next);
        return res;
    }

    bool try_pop(T &value)
    {
        lock_guard<mutex> lk(head_mutex);
        if (head.get() == tail)
            return false;

        value = move(head->data);
        head = move(head->next);
        return true;
    }

    shared_ptr<T> wait_and_pop()
    {
        unique_lock<mutex> lk(head_mutex);
        while (head.get() == tail)
            data_cond.wait(lk);

        shared_ptr<T> res(make_shared<T>(move(head->data)));
        head = move(head->next);
        return res;
    }

    void wait_and_pop(T &value)
    {
        unique_lock<mutex> lk(head_mutex);
        while (head.get() == tail)
            data_cond.wait(lk);

        value = move(head->data);
        head = move(head->next);
    }

    void push(T new_value)
    {
        shared_ptr<T> new_data(make_shared<T>(move(new_value)));
        unique_ptr<node> p(new node);
        node *const new_tail = p.get();
        {
            lock_guard<mutex> tail_lock(tail_mutex);
            tail->data = new_data;
            tail->next = move(p);
            tail = new_tail;
        }
        data_cond.notify_one();
    }

    bool empty();
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
