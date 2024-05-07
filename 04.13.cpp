#include <list>
#include <algorithm>
#include <future>
#include <iostream>
using namespace std;

template <typename T>
list<T> parallel_quick_sort(list<T> input)
{
    if (input.empty())
        return input;

    list<T> result;
    result.splice(result.begin(), input, input.begin());

    T const &pivot = *result.begin();
    auto divide_point = partition(input.begin(), input.end(),
                                  [&](T const &t)
                                  {
                                      return t < pivot;
                                  });
    list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point);

    future<list<T>> new_lower(async(&parallel_quick_sort<T>, move(lower_part)));
    auto new_higher(parallel_quick_sort(move(input)));

    result.splice(result.end(), new_higher);
    result.splice(result.begin(), new_lower.get());
    return result;
}

int main()
{
    list<int> input{45, 84, 22, 16, 0, 32, 15, 11, 6, 3, 9, 12, 0};
    list<int> result = parallel_quick_sort(input);
    for (int data : result)
        cout << data << endl;
    return 0;
}
