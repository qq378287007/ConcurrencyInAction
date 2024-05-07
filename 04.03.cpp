#include <memory>
using namespace std;

template <typename T>
class threadsafe_queue
{
public:
    threadsafe_queue();
    threadsafe_queue(const threadsafe_queue &);
    threadsafe_queue &operator=(const threadsafe_queue &) = delete;

    void push(T new_value);

    bool try_pop(T &value);
    shared_ptr<T> try_pop();

    void wait_and_pop(T &value);
    shared_ptr<T> wait_and_pop();

    bool empty() const;
};

int main()
{
    return 0;
}
