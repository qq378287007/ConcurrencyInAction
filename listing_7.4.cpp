#include <atomic>
#include <memory>
using namespace std;

template <typename T>
class lock_free_stack
{
private:
    struct node
    {
        T data;
        node *next;
        node(T const &data_) : data(data_) {}
    };
    atomic<node *> head;

private:
    atomic<unsigned> threads_in_pop;
    void try_reclaim(node *old_head) {}

public:
    shared_ptr<T> pop()
    {
        ++threads_in_pop;
        node *old_head = head.load();
        while (old_head && !head.compare_exchange_weak(old_head, old_head->next))
            ;
        shared_ptr<T> res;
        if (old_head)
            res.swap(old_head->data);

        try_reclaim(old_head);
        return res;
    }
};

int main()
{
    return 0;
}
