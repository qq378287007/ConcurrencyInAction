#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
using namespace std;

template <typename T>
class threadsafe_queue
{
private:
    mutable mutex mut;
    queue<T> data_queue;
    condition_variable data_cond;

public:
    threadsafe_queue()
    {
    }

    void push(T new_value)
    {
        lock_guard<mutex> lk(mut);
        data_queue.push(move(new_value));
        data_cond.notify_one();
    }

    void wait_and_pop(T &value)
    {
        unique_lock<mutex> lk(mut);
        // data_cond.wait(lk, [this] { return !data_queue.empty(); });
        while (data_queue.empty())
            data_cond.wait(lk);
        value = move(data_queue.front());
        data_queue.pop();
    }

    shared_ptr<T> wait_and_pop()
    {
        unique_lock<mutex> lk(mut);
        // data_cond.wait(lk, [this] { return !data_queue.empty(); });
        while (data_queue.empty())
            data_cond.wait(lk);
        shared_ptr<T> res(make_shared<T>(move(data_queue.front())));
        data_queue.pop();
        return res;
    }

    bool try_pop(T &value)
    {
        lock_guard<mutex> lk(mut);
        if (data_queue.empty())
            return false;
        value = move(data_queue.front());
        data_queue.pop();
    }

    shared_ptr<T> try_pop()
    {
        lock_guard<mutex> lk(mut);
        if (data_queue.empty())
            return shared_ptr<T>();
        shared_ptr<T> res(make_shared<T>(move(data_queue.front())));
        data_queue.pop();
        return res;
    }

    bool empty() const
    {
        lock_guard<mutex> lk(mut);
        return data_queue.empty();
    }
};

int main()
{
    threadsafe_queue<int> rq;
    return 0;
}
