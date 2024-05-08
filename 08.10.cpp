#include <atomic>
#include <future>
using namespace std;

template <typename Iterator, typename MatchType>
Iterator parallel_find_impl(Iterator first, Iterator last, MatchType match, atomic<bool> &done)
{
    try
    {
        unsigned long const length = distance(first, last);
        unsigned long const min_per_thread = 25;
        if (length < (2 * min_per_thread))
        {
            for (; (first != last) && !done.load(); ++first)
            {
                if (*first == match)
                {
                    done = true;
                    return first;
                }
            }
            return last;
        }
        else
        {
            Iterator const mid_point = first + (length / 2);
            future<Iterator> async_result =
                async(&parallel_find_impl<Iterator, MatchType>,
                      mid_point, last, match, ref(done));
            Iterator const direct_result =
                parallel_find_impl(first, mid_point, match, done);
            return (direct_result == mid_point) ? async_result.get() : direct_result;
        }
    }
    catch (...)
    {
        done = true;
        throw;
    }
}

template <typename Iterator, typename MatchType>
Iterator parallel_find(Iterator first, Iterator last, MatchType match)
{
    atomic<bool> done(false);
    return parallel_find_impl(first, last, match, done);
}
