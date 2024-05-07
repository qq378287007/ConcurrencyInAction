#include <list>
#include <algorithm>
#include <iostream>
using namespace std;

template <typename T>
list<T> sequential_quick_sort(list<T> input)
{
    if (input.empty())
        return input;

    // input第一个元素剪切到result
    list<T> result;
    result.splice(result.begin(), input, input.begin());

    // input划分为两部分（未排序），小于pivot的位于前面，divide_point为大于等于pivot的迭代器
    T const &pivot = *result.begin();
    auto divide_point = partition(input.begin(), input.end(),
                                  [&](T const &t)
                                  {
                                      return t < pivot;
                                  });

    // input前面元素（小于pivot）剪切到lower_part中
    list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point);

    // 两个list分别递归排序
    auto new_lower(sequential_quick_sort(move(lower_part)));
    auto new_higher(sequential_quick_sort(move(input)));

    // result前面插入小元素
    result.splice(result.begin(), new_lower);
    // result后面插入大元素
    result.splice(result.end(), new_higher);
    return result;
}

int main()
{
    list<int> input{45, 84, 22, 16, 0, 32, 15, 11, 6, 3, 9, 12, 0};
    list<int> result = sequential_quick_sort(input);
    for(int data: result)
        cout << data <<endl;
    
    return 0;
}
