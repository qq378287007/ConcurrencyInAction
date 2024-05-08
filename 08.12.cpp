#include <thread>
#include <atomic>
using namespace std;

class barrier
{
    unsigned const count;
    atomic<unsigned> spaces;
    atomic<unsigned> generation;

public:
    explicit barrier(unsigned count_) : count(count_), spaces(count), generation(0)
    {
    }
    void wait()
    {
        unsigned const my_generation = generation;
        if (!--spaces)
        {
            spaces = count;
            ++generation;
        }
        else
        {
            while (generation == my_generation)
                this_thread::yield();
        }
    }
};
