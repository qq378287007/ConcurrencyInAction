#include <deque>
#include <mutex>
#include <future>
#include <thread>
#include <utility>
#include <iostream>
using namespace std;

mutex m;
deque<packaged_task<void()>> tasks;

bool gui_shutdown_message_received() { return false; }
void get_and_process_gui_message() {}

void gui_thread()
{
    while (!gui_shutdown_message_received())
    {
        get_and_process_gui_message();
        packaged_task<void()> task;
        {
            lock_guard<mutex> lk(m);
            if (tasks.empty())
                continue;
            task = move(tasks.front());
            tasks.pop_front();
        }
        task();
    }
}

template <typename Func>
future<void> post_task_for_gui_thread(Func f)
{
    packaged_task<void()> task(f);
    future<void> res = task.get_future();
    lock_guard<mutex> lk(m);
    tasks.push_back(move(task));
    return res;
}

void func()
{
    static int i = 0;
    cout << ++i << endl;
}

void add_task()
{
    while (true)
    {
        post_task_for_gui_thread(func);
        this_thread::sleep_for(chrono::seconds(1));
    }
}

int main()
{
    thread gui_bg_thread(gui_thread);
    thread add_task_thread(add_task);
    add_task_thread.join();
    gui_bg_thread.join();

    return 0;
}
