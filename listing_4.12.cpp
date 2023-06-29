#include <list>
#include <algorithm>
using namespace std;

template <typename T>
list<T> sequential_quick_sort(list<T> input)
{
    if (input.empty())
        return input;
    
    list<T> result;
    result.splice(result.begin(), input, input.begin());
    T const &pivot = *result.begin();
    auto divide_point = partition(input.begin(), input.end(),
                                  [&](T const &t)
                                  { return t < pivot; });
    list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(),
                      divide_point);
    auto new_lower(
        sequential_quick_sort(move(lower_part)));
    auto new_higher(
        sequential_quick_sort(move(input)));
    result.splice(result.end(), new_higher);
    result.splice(result.begin(), new_lower);
    return result;
}

int main(){
    return 0;
}