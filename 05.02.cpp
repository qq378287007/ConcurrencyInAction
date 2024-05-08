#include <vector>
#include <atomic>
#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

vector<int> vector_data;
atomic_bool data_ready(false);

void reader_thread()
{
    while (!data_ready.load())
        this_thread::sleep_for(chrono::milliseconds(1));

    cout << "The answer = " << vector_data[0] << "\n";
}

void writer_thread()
{
    vector_data.push_back(42);
    data_ready = true;
}

int main()
{
    thread a(reader_thread);
    thread b(writer_thread);
    b.join();
    a.join();

    return 0;
}
