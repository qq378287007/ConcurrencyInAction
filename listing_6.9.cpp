#include <memory>
#include <mutex>
#include <atomic>
#include <condition_variable>

// #include "node.hpp"
template <typename T>
struct node
{
    //T data;
    //node<T>  *next;

    std::shared_ptr<T> data;
    std::unique_ptr<node<T>> next;
    //node() : next() {}
    //node(T const &value) : data(std::make_shared<T>(value)) {}
};

template <typename T>
class threadsafe_queue
{
private:
    std::atomic<node<T> *> tail;
    std::atomic<node<T> *> head;
    std::mutex tail_mutex;
    std::mutex head_mutex;
    std::condition_variable data_cond;
    node<T> *get_tail()
    {
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        return tail;
    }

    std::unique_ptr<node<T>> pop_head()
    {
        std::unique_ptr<node<T>> const old_head = std::move(head);
        head = std::move(old_head->next);
        return old_head;
    }

    std::unique_lock<std::mutex> wait_for_data()
    {
        std::unique_lock<std::mutex> head_lock(head_mutex);
        data_cond.wait(head_lock, [&]
                       { return head != get_tail(); });
        return std::move(head_lock);
    }

    std::unique_ptr<node<T>> wait_pop_head()
    {
        std::unique_lock<std::mutex> head_lock(wait_for_data());
        return pop_head();
    }

    std::unique_ptr<node<T>> wait_pop_head(T &value)
    {
        std::unique_lock<std::mutex> head_lock(wait_for_data());
        value = std::move(*head->data);
        return pop_head();
    }

public:
    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_ptr<node<T>> const old_head = wait_pop_head();
        return old_head->data;
    }

    void wait_and_pop(T &value)
    {
        std::unique_ptr<node<T>> const old_head = wait_pop_head(value);
    }
};

int main()
{
    return 0;
}
