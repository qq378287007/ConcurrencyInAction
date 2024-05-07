#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
using namespace std;

template <typename T>
class threadsafe_queue
{
    mutex mut;
    queue<T> data_queue;
    condition_variable data_cond;

public:
    void push(T new_value)
    {
        {
            lock_guard<mutex> lk(mut);
            data_queue.push(new_value);
        }
        data_cond.notify_one();
    }

    void wait_and_pop(T &value)
    {
        unique_lock<mutex> lk(mut);
        // data_cond.wait(lk, [this] { return !data_queue.empty(); });
        while (data_queue.empty())
            data_cond.wait(lk);

        value = data_queue.front();
        data_queue.pop();
    }
};

struct data_chunk
{
};

bool more_data_to_prepare() { return false; }
data_chunk prepare_data() { return data_chunk(); }
void process(data_chunk &) {}
bool is_last_chunk(data_chunk &) { return true; }

threadsafe_queue<data_chunk> data_queue;

void add_data()
{
    data_chunk const data = prepare_data();
    data_queue.push(data);
}

void data_preparation_thread()
{
    while (more_data_to_prepare())
        add_data();
}

void data_processing_thread()
{
    data_chunk data;
    while (true)
    {
        data_queue.wait_and_pop(data);

        if (is_last_chunk(data))
            break;
        process(data);
    }
}

int main()
{
    thread t1(data_preparation_thread);
    thread t2(data_processing_thread);

    t1.join();
    add_data();
    t2.join();

    return 0;
}
