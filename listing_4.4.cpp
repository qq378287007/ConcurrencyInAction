#include <mutex>
#include <condition_variable>
#include <queue>
using namespace std;

template <typename T>
class threadsafe_queue
{
private:
    mutex mut;
    queue<T> data_queue;
    condition_variable data_cond;

public:
    void push(T new_value)
    {
        lock_guard<mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }

    void wait_and_pop(T &value)
    {
        unique_lock<mutex> lk(mut);
        data_cond.wait(lk, [this]
                       { return !data_queue.empty(); });
        value = data_queue.front();
        data_queue.pop();
    }
};

struct data_chunk
{
};

data_chunk prepare_data()
{
    return data_chunk{};
}

bool more_data_to_prepare()
{
    return true;
}

void process(data_chunk)
{
}

bool is_last_chunk(data_chunk)
{
    return true;
}

threadsafe_queue<data_chunk> data_queue;

void data_preparation_thread()
{
    while (more_data_to_prepare())
    {
        data_chunk const data = prepare_data();
        data_queue.push(data);
    }
}

void data_processing_thread()
{
    while (true)
    {
        data_chunk data;
        data_queue.wait_and_pop(data);
        process(data);
        if (is_last_chunk(data))
            break;
    }
}

int main()
{
    return 0;
}