#include <future>
#include <algorithm>
using namespace std;

template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
    unsigned long const length = distance(first, last);
    unsigned long const max_chunk_size = 25;
    if (length <= max_chunk_size)
        return accumulate(first, last, init);

    Iterator mid_point = first;
    advance(mid_point, length / 2);
    future<T> first_half_result = async(parallel_accumulate<Iterator, T>, first, mid_point, init);
    T second_half_result = parallel_accumulate(mid_point, last, T());
    return first_half_result.get() + second_half_result;
}

int main()
{
    return 0;
}
