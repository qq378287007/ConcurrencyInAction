#include <future>
#include <iostream>
using namespace std;

int find_the_answer_to_ltuae() { return 42; }

void do_other_stuff() {}

int main()
{
    future<int> the_answer = async(find_the_answer_to_ltuae);
    do_other_stuff();

    the_answer.wait(); // wait可省略
    the_answer.wait(); // wait可省略
    cout << "The answer is " << the_answer.get() << endl;
    // cout << "The answer is " << the_answer.get() << endl;//error，get只能调用一次
    return 0;
}
