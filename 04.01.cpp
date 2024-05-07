#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
using namespace std;

struct data_chunk
{
};

mutex mut;
queue<data_chunk> data_queue;
condition_variable data_cond;

bool more_data_to_prepare() { return false; }
data_chunk prepare_data() { return data_chunk(); }
void process(data_chunk &) {}
bool is_last_chunk(data_chunk &) { return true; }

void add_data()
{
    data_chunk const data = prepare_data();
    {
        lock_guard<mutex> lk(mut);
        data_queue.push(data);
    }
    data_cond.notify_one();
}

void data_preparation_thread()
{
    while (more_data_to_prepare())
        add_data();
}

void data_processing_thread()
{
    while (true)
    {
        unique_lock<mutex> lk(mut);
        // data_cond.wait(lk, [] { return !data_queue.empty(); });
        while (data_queue.empty())
            data_cond.wait(lk);
        data_chunk data = data_queue.front();
        data_queue.pop();
        lk.unlock();

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
