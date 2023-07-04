#include <thread>
#include <numeric>
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>
using namespace std;

template <typename Iterator, typename T>
struct accumulate_block
{
    void operator()(Iterator first, Iterator last, T &result)
    {
        result = accumulate(first, last, result);
    }
};

template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
    unsigned long const length = distance(first, last);

    if (!length)
        return init;

    // 每个线程处理元素的最低限定量
    unsigned long const min_per_thread = 25;

    // 最大线程数量
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;

    // 硬件线程数量
    unsigned long const hardware_threads = thread::hardware_concurrency();

    // 实际运行线程数量
    unsigned long const num_threads = min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

    // 各线程分担数量
    unsigned long const block_size = length / num_threads;

    vector<T> results(num_threads);

    // 发起线程数量，当前函数占据一个线程
    vector<thread> threads(num_threads - 1);

    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i)
    {
        Iterator block_end = block_start;
        advance(block_end, block_size);
        threads[i] = thread(accumulate_block<Iterator, T>(),
                            block_start, block_end, ref(results[i]));
        block_start = block_end;
    }
    // 处理最后一小块
    accumulate_block<Iterator, T>()(block_start, last, results[num_threads - 1]);

    // 循环等待所有线程完成
    for_each(threads.begin(), threads.end(), mem_fn(&thread::join));

    // 数值累加
    return accumulate(results.begin(), results.end(), init);
}

int main()
{
    vector<int> vi;
    for (int i = 0; i < 10; ++i)
        vi.push_back(10);

    int sum = parallel_accumulate(vi.begin(), vi.end(), 5);
    cout << "sum=" << sum << endl;
    return 0;
}
