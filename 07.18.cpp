#include <memory>
#include <atomic>
using namespace std;

template<typename T>
class lock_free_queue
{
private:    struct node;
    struct node_counter
    {
        unsigned internal_count:30;
        unsigned external_counters:2;
    };

    struct counted_node_ptr
    {
        int external_count;
        node* ptr;
    };
    struct node
    {
        shared_ptr<T> data;
        atomic<int> internal_count;
        counted_node_ptr next;
        node(T const& data_):
            data(make_shared<T>(data_)),
            internal_count(0)
        {}
        void release_ref()
        {
            node_counter old_counter=
                count.load(memory_order_relaxed);
            node_counter new_counter;
            do
            {
                new_counter=old_counter;
                --new_counter.internal_count;
            }
            while(!count.compare_exchange_strong(
                      old_counter,new_counter,
                      memory_order_acquire,memory_order_relaxed));
            if(!new_counter.internal_count &&
               !new_counter.external_counters)
            {
                delete this;
            }
        }
    };
    atomic<counted_node_ptr> head;
    atomic<counted_node_ptr> tail;


};
