#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <string>
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
        explicit wrapped_message(Msg const &contents_) : contents(contents_) {}
    };

    class queueMy
    {
        mutex m;
        condition_variable c;
        queue<shared_ptr<message_base>> q;

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
    };
}

using namespace messaging;
int main()
{
    queueMy q;

    q.push(123);
    auto d1 = q.wait_and_pop();
    auto d11 = dynamic_pointer_cast<wrapped_message<int>>(d1);

    q.push(string("123"));
    auto d2 = q.wait_and_pop();
    auto d22 = dynamic_pointer_cast<wrapped_message<string>>(d2);
    return 0;
}
