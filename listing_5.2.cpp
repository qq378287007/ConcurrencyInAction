#include <vector>
#include <atomic>
#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

vector<int> data;
atomic_bool data_ready(false);

void reader_thread()
{
    while (!data_ready.load())
        this_thread::sleep_for(chrono::milliseconds(1));

    cout << "The answer=" << data[0] << "\n";
}

void writer_thread()
{
    data.push_back(42);
    data_ready = true;
}

int main()
{
    return 0;
}