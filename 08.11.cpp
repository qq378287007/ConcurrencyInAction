#include <future>
#include <algorithm>
#include <vector>
using namespace std;

struct join_threads
{
    join_threads(vector<thread> &) {}
};

template <typename Iterator>
void parallel_partial_sum(Iterator first, Iterator last)
{
    typedef typename Iterator::value_type value_type;
    struct process_chunk
    {
        void operator()(Iterator begin, Iterator last,
                        future<value_type> *previous_end_value,
                        promise<value_type> *end_value)
        {
            try
            {
                Iterator end = last;
                ++end;
                partial_sum(begin, end, begin);
                if (previous_end_value)
                {
                    value_type &addend = previous_end_value->get();
                    *last += addend;
                    if (end_value)
                    {
                        end_value->set_value(*last);
                    }
                    for_each(begin, last, [addend](value_type &item)
                             { item += addend; });
                }
                else if (end_value)
                {
                    end_value->set_value(*last);
                }
            }
            catch (...)
            {
                if (end_value)
                {
                    end_value->set_exception(current_exception());
                }
                else
                {
                    throw;
                }
            }
        }
    };

    unsigned long const length = distance(first, last);

    if (!length)
        return last;

    unsigned long const min_per_thread = 25;
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;

    unsigned long const hardware_threads = thread::hardware_concurrency();

    unsigned long const num_threads = min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

    unsigned long const block_size = length / num_threads;

    typedef typename Iterator::value_type value_type;

    vector<thread> threads(num_threads - 1);
    vector<promise<value_type>> end_values(num_threads - 1);
    vector<future<value_type>> previous_end_values;
    previous_end_values.reserve(num_threads - 1);
    join_threads joiner(threads);

    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i)
    {
        Iterator block_last = block_start;
        advance(block_last, block_size - 1);
        threads[i] = thread(process_chunk(),
                            block_start, block_last,
                            (i != 0) ? &previous_end_values[i - 1] : 0,
                            &end_values[i]);
        block_start = block_last;
        ++block_start;
        previous_end_values.push_back(end_values[i].get_future());
    }
    Iterator final_element = block_start;
    advance(final_element, distance(block_start, last) - 1);
    process_chunk()(block_start, final_element,
                    (num_threads > 1) ? &previous_end_values.back() : 0,
                    0);
}
