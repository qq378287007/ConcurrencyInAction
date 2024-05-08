#include <thread>
#include <vector>
#include <future>
#include <list>
using namespace std;

template <typename T>
struct sorter
{
    struct chunk_to_sort
    {
        list<T> data;
        promise<list<T>> promise;
    };

    thread_safe_stack<chunk_to_sort> chunks;
    vector<thread> threads;
    unsigned const max_thread_count;
    atomic<bool> end_of_data;

    sorter() : max_thread_count(thread::hardware_concurrency() - 1), end_of_data(false) {}
    ~sorter()
    {
        end_of_data = true;
        for (unsigned i = 0; i < threads.size(); ++i)
            threads[i].join();
    }

    void try_sort_chunk()
    {
        boost::shared_ptr<chunk_to_sort> chunk = chunks.pop();
        if (chunk)
            sort_chunk(chunk);
    }

    list<T> do_sort(list<T> &chunk_data)
    {
        if (chunk_data.empty())
            return chunk_data;
            
        list<T> result;
        result.splice(result.begin(), chunk_data, chunk_data.begin());
        T const &partition_val = *result.begin();

        typename list<T>::iterator divide_point =
            partition(chunk_data.begin(), chunk_data.end(),
                           [&](T const &val)
                           { return val < partition_val; });
        chunk_to_sort new_lower_chunk;
        new_lower_chunk.data.splice(new_lower_chunk.data.end(),
                                    chunk_data, chunk_data.begin(),
                                    divide_point);

        future<list<T>> new_lower = new_lower_chunk.promise.get_future();
        chunks.push(move(new_lower_chunk));
        if (threads.size() < max_thread_count)
            threads.push_back(thread(&sorter<T>::sort_thread, this));

        list<T> new_higher(do_sort(chunk_data));

        result.splice(result.end(), new_higher);
        while (new_lower.wait_for(chrono::seconds(0)) != future_status::ready)
            try_sort_chunk();
        

        result.splice(result.begin(), new_lower.get());
        return result;
    }

    void sort_chunk(boost::shared_ptr<chunk_to_sort> const &chunk)
    {
        chunk->promise.set_value(do_sort(chunk->data));
    }

    void sort_thread()
    {
        while (!end_of_data)
        {
            try_sort_chunk();
            this_thread::yield();
        }
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
