#include <memory>
#include <atomic>
using namespace std;

template<typename T>
class lock_free_queue
{
private:
#include "node.hpp"
    struct counted_node_ptr
    {
        int external_count;
        node* ptr;
    };
    
    atomic<counted_node_ptr> head;
    atomic<counted_node_ptr> tail;
public:
    unique_ptr<T> pop()
    {
        counted_node_ptr old_head=head.load(memory_order_relaxed);
        for(;;)
        {
            increase_external_count(head,old_head);
            node* const ptr=old_head.ptr;
            if(ptr==tail.load().ptr)
            {
                ptr->release_ref();
                return unique_ptr<T>();
            }
            if(head.compare_exchange_strong(old_head,ptr->next))
            {
                T* const res=ptr->data.exchange(nullptr);
                free_external_counter(old_head);
                return unique_ptr<T>(res);
            }
            ptr->release_ref();
        }
    }
};
