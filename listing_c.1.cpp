#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
using namespace std;

namespace messaging
{
    struct message_base
    {
        virtual ~message_base() = default;
    };

    template <typename Msg>
    struct wrapped_message : message_base
    {
        Msg contents;
        explicit wrapped_message(Msg const &contents_) : contents(contents_)
        {
        }
    };

    class queueMy
    {
    public:
        template <typename T>
        void push(T const &msg)
        {
            lock_guard<mutex> lk(m);
            q.push(make_shared<wrapped_message<T>>(msg));
            c.notify_all();
        }
        shared_ptr<message_base> wait_and_pop()
        {
            unique_lock<mutex> lk(m);
            // c.wait(lk, [&]{ return !q.empty(); });
            while (q.empty())
                c.wait(lk);

            auto res = q.front();
            q.pop();
            return res;
        }

    private:
        mutex m;
        condition_variable c;
        queue<shared_ptr<message_base>> q;
    };
}

int main()
{
    return 0;
}