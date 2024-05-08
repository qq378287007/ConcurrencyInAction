#include <memory>
#include <iostream>
using namespace std;

template <typename T>
class queue
{
    struct node
    {
        shared_ptr<T> data;
        unique_ptr<node> next;
    };

    unique_ptr<node> head;
    node *tail;

public:
    queue() : head(new node), tail(head.get()) {}

    queue(const queue &other) = delete;
    queue &operator=(const queue &other) = delete;

    shared_ptr<T> try_pop()
    {
        if (head.get() == tail)
            return shared_ptr<T>();

        shared_ptr<T> const res(head->data);
        unique_ptr<node> const old_head = move(head);
        head = move(old_head->next);
        return res;
    }

    void push(T new_value)
    {
        shared_ptr<T> new_data(make_shared<T>(move(new_value)));
        tail->data = new_data;

        unique_ptr<node> p(new node);
        node *const new_tail = p.get();
        tail->next = move(p);
        tail = new_tail;
    }
};

int main()
{
    queue<int> q;
    q.push(1);
    q.push(2);

    auto d1 = q.try_pop();
    cout << *d1 << endl;

    auto d2 = q.try_pop();
    cout << *d2 << endl;

    cout << boolalpha << (q.try_pop() == nullptr) << endl;

    return 0;
}
