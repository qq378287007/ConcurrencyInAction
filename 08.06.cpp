#include <future>
#include <algorithm>
#include <thread>
using namespace std;

thread task_thread;
atomic<bool> task_cancelled(false);

enum TYPE
{
    start_task,
    stop_task,
    task_complete0,
    quit,
};
struct event_data
{
    TYPE type;
};
void display_results() {}
event_data get_event()
{
    event_data e;
    e.type = quit;
    return e;
}

bool task_complete()
{
    return true;
}
void do_next_operation() {}
void perform_cleanup() {}
void post_gui_event(TYPE type) {}
void task()
{
    while (!task_complete() && !task_cancelled)
        do_next_operation();

    if (task_cancelled)
        perform_cleanup();
    else
        post_gui_event(task_complete0);
}

void process(event_data const &event)
{
    switch (event.type)
    {
    case start_task:
        task_cancelled = false;
        task_thread = thread(task);
        break;
    case stop_task:
        task_cancelled = true;
        task_thread.join();
        break;
    case task_complete0:
        task_thread.join();
        display_results();
        break;
    default:
        break;
    }
}

void gui_thread()
{
    while (true)
    {
        event_data event = get_event();
        if (event.type == quit)
            break;
        process(event);
    }
}

int main()
{
    return 0;
}
