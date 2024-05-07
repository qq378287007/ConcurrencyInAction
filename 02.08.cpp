#include <vector>
#include <thread>
#include <algorithm>
#include <functional>
#include <iostream>
using namespace std;

void do_work(unsigned id) {}
void f(unsigned n)
{
    vector<thread> threads;
    for (unsigned i = 0; i < n; ++i)
        // threads.push_back(thread(do_work, i));
        threads.emplace_back(thread(do_work, i));

    // for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
    for (thread &t : threads)
        t.join();
}

void do_work2(int *data, unsigned s_id, unsigned e_id, int &result)
{
    result = 0;
    for (unsigned i = s_id; i <= e_id; i++)
        result += data[i];
}
int f2(int *data, unsigned len, unsigned n)
{
    vector<int> results;
    results.resize(n);

    vector<thread> threads;
    for (unsigned i = 0; i < n; ++i)
    {
        unsigned s_id = len * i / n;
        unsigned e_id = len * (i + 1) / n - 1;
        threads.push_back(thread(do_work2, data, s_id, e_id, ref(results[i])));
    }
    for_each(threads.begin(), threads.end(), mem_fn(&thread::join));

    int result = 0;
    for (unsigned i = 0; i < n; ++i)
        result += results[i];

    return result;
}

int main()
{
    unsigned n = 20;
    f(n);

    unsigned len = 100;
    int data[100];
    for (unsigned i = 0; i < len; ++i)
        data[i] = i;
    int result = f2(data, len, n);
    cout << "result: " << result << endl;

    return 0;
}
