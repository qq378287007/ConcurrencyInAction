#include <list>
#include <algorithm>
#include <vector>
using namespace std;

template <typename T>
struct sorter
{
    thread_pool pool;

    list<T> do_sort(list<T> &chunk_data)
    {
        if (chunk_data.empty())
            return chunk_data;
            
        list<T> result;
        result.splice(result.begin(), chunk_data, chunk_data.begin());
        T const &partition_val = *result.begin();

        typename list<T>::iterator divide_point =
            partition(
                chunk_data.begin(), chunk_data.end(),
                [&](T const &val)
                { return val < partition_val; });

        list<T> new_lower_chunk;
        new_lower_chunk.splice(
            new_lower_chunk.end(),
            chunk_data, chunk_data.begin(),
            divide_point);

        thread_pool::task_handle<list<T>> new_lower =
            pool.submit(
                bind(
                    &sorter::do_sort, this,
                    move(new_lower_chunk)));

        list<T> new_higher(do_sort(chunk_data));

        result.splice(result.end(), new_higher);
        while (!new_lower.is_ready())
            pool.run_pending_task();
            
        result.splice(result.begin(), new_lower.get());
        return result;
    }
};

template <typename T>
list<T> parallel_quick_sort(list<T> input)
{
    if (input.empty())
        return input;
    
    sorter<T> s;
    return s.do_sort(input);
}
