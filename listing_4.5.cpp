#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <thread>
#include <functional>
#include <iostream>
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
    threadsafe_queue(threadsafe_queue const &other)
    {
        lock_guard<mutex> lk(other.mut);
        data_queue = other.data_queue;
    }

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

    shared_ptr<T> wait_and_pop()
    {
        unique_lock<mutex> lk(mut);
        // data_cond.wait(lk, [this] { return !data_queue.empty(); });
        while (data_queue.empty())
            data_cond.wait(lk);

        shared_ptr<T> res(make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }

    bool try_pop(T &value)
    {
        lock_guard<mutex> lk(mut);
        if (data_queue.empty())
            return false;

        value = data_queue.front();
        data_queue.pop();
        return true;
    }

    shared_ptr<T> try_pop()
    {
        lock_guard<mutex> lk(mut);
        if (data_queue.empty())
            return shared_ptr<T>();

        shared_ptr<T> res(make_shared<T>(data_queue.front()));
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
    threadsafe_queue<int> q;

    thread t1(&threadsafe_queue<int>::push, &q, 1);
    thread t2(&threadsafe_queue<int>::push, &q, 2);

    int data = 0;
    using Func = void (threadsafe_queue<int>::*)(int &);
    Func f1 = &threadsafe_queue<int>::wait_and_pop;
    thread t3(bind(f1, &q, ref(data)));

    // shared_ptr<int> (threadsafe_queue<int>::*f2)() = &threadsafe_queue<int>::wait_and_pop;
    // thread t4(bind(f2, &q));

    shared_ptr<int> data2;
    thread t4([&q, &data2] { data2 = q.wait_and_pop(); });

    t1.join();
    t2.join();

    t3.join();
    t4.join();

    cout << "data=" << data << endl;
    cout << "data2=" << *data2 << endl;

    return 0;
}
