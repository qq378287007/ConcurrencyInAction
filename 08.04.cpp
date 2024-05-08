#include <iterator>
#include <vector>
#include <thread>
#include <future>
#include <numeric>
using namespace std;

struct join_threads
{
    join_threads(vector<thread> &) {}
};

template <typename Iterator, typename T>
struct accumulate_block
{
    T operator()(Iterator first, Iterator last)
    {
        return accumulate(first, last, T());
    }
};
template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
    unsigned long const length = distance(first, last);

    if (!length)
        return init;

    unsigned long const min_per_thread = 25;
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;

    unsigned long const hardware_threads = thread::hardware_concurrency();

    unsigned long const num_threads = min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

    unsigned long const block_size = length / num_threads;

    vector<future<T>> futures(num_threads - 1);
    vector<thread> threads(num_threads - 1);
    join_threads joiner(threads);

    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i)
    {
        Iterator block_end = block_start;
        advance(block_end, block_size);
        packaged_task<T(Iterator, Iterator)> task(accumulate_block<Iterator, T>());
        futures[i] = task.get_future();
        threads[i] = thread(move(task), block_start, block_end);
        block_start = block_end;
    }
    T last_result = accumulate_block<Iterator, T>(block_start, last);
    T result = init;
    for (unsigned long i = 0; i < (num_threads - 1); ++i)
        result += futures[i].get();

    result += last_result;
    return result;
}
