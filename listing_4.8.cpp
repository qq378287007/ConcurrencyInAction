#include <string>
#include <vector>
#include <future>
#include <iostream>
using namespace std;

/*
template <>
class packaged_task<string(vector<char> *, int)>
{
public:
    template <typename Callable>
    explicit packaged_task(Callable &&f);

    future<string> get_future();
    void operator()(vector<char> *, int);
};
*/

string func(vector<char> *, int &)
{
    return "out";
}

int main()
{
    packaged_task<string(vector<char> *, int &)> task(func);
    future<string> res = task.get_future();

    vector<char> *a;
    int b;
    thread t(std::move(task), a, std::ref(b));

    string result = res.get();
    cout <<"result: " <<result <<endl;

    t.join();

    return 0;
}
