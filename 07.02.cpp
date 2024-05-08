#include <atomic>
using namespace std;

template <typename T>
class lock_free_stack
{
    struct node
    {
        T data;
        node *next;
        node(T const &data_) : data(data_) {}
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
};

int main()
{
    return 0;
}
