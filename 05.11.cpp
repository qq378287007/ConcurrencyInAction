#include <atomic>
#include <thread>
#include <vector>
using namespace std;

vector<int> queue_data;
atomic<int> atomic_count;

void wait_for_more_items() {}
void process(int data) {}

void populate_queue()
{
    unsigned const number_of_items = 20;
    queue_data.clear();
    for (unsigned i = 0; i < number_of_items; ++i)
        queue_data.push_back(i);

    atomic_count.store(number_of_items, memory_order_release);
}

void consume_queue_items()
{
    while (true)
    {
        int item_index;
        if ((item_index = atomic_count.fetch_sub(1, memory_order_acquire)) <= 0)
        {
            wait_for_more_items();
            continue;
        }
        process(queue_data[item_index - 1]);
    }
}

int main()
{
    thread a(populate_queue);
    thread b(consume_queue_items);
    thread c(consume_queue_items);
    a.join();
    b.join();
    c.join();
    return 0;
}
