#include <future>
#include <iostream>
using namespace std;

int find_the_answer_to_ltuae()
{
    return 42;
}

void do_other_stuff()
{
}

int main()
{
    future<int> the_answer = async(find_the_answer_to_ltuae);
    do_other_stuff();
    cout << "The answer is " << the_answer.get() << endl;
    return 0;
}
