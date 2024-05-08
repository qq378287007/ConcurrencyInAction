#include <atomic>
#include <thread>
using namespace std;

unsigned const max_hazard_pointers = 100;
struct hazard_pointer
{
    atomic<thread::id> id;
    atomic<void *> pointer;
};
hazard_pointer hazard_pointers[max_hazard_pointers];
class hp_owner
{
    hazard_pointer *hp;

public:
    hp_owner(hp_owner const &) = delete;
    hp_owner operator=(hp_owner const &) = delete;
    hp_owner() : hp(nullptr)
    {
        for (unsigned i = 0; i < max_hazard_pointers; ++i)
        {
            thread::id old_id;
            if (hazard_pointers[i].id.compare_exchange_strong(old_id, this_thread::get_id()))
            {
                hp = &hazard_pointers[i];
                break;
            }
        }
        if (!hp)
            throw runtime_error("No hazard pointers available");
    }
    atomic<void *> &get_pointer()
    {
        return hp->pointer;
    }
    ~hp_owner()
    {
        hp->pointer.store(nullptr);
        hp->id.store(thread::id());
    }
};
atomic<void *> &get_hazard_pointer_for_current_thread()
{
    thread_local static hp_owner hazard;
    return hazard.get_pointer();
}

int main()
{
    return 0;
}
