#include <atomic>
#include <memory>
using namespace std;

template <typename T>
class lock_free_stack
{
private:
    struct node
    {
        shared_ptr<T> data;
        node *next;
        node(T const &data_) : data(make_shared<T>(data_)) {}
    };
    atomic<node *> head;

public:
    void push(T const &data)
    {
        node *const new_node = new node(data);
        new_node->next = head.load();
        while (!head.compare_exchange_weak(new_node->next, new_node))
            ;
    }
    shared_ptr<T> pop()
    {
        node *old_head = head.load();
        while (old_head && !head.compare_exchange_weak(old_head, old_head->next))
            ;
        return old_head ? old_head->data : shared_ptr<T>();
    }
};

int main()
{
    return 0;
}
