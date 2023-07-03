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

    class close_queue
    {
    };

    class dispatcher
    {
        queueMy *q;
        bool chained;

        dispatcher(dispatcher const &) = delete;
        dispatcher &operator=(dispatcher const &) = delete;

        template <typename Dispatcher, typename Msg, typename Func>
        friend class TemplateDispatcher;

        void wait_and_dispatch()
        {
            for (;;)
            {
                auto msg = q->wait_and_pop();
                dispatch(msg);
            }
        }

        bool dispatch(std::shared_ptr<message_base> const &msg)
        {
            if (dynamic_cast<wrapped_message<close_queue> *>(msg.get()))
                throw close_queue();

            return false;
        }

    public:
        dispatcher(dispatcher &&other) : q(other.q), chained(other.chained)
        {
            other.chained = true;
        }

        explicit dispatcher(queueMy *q_) : q(q_), chained(false)
        {
        }

        template <typename Message, typename Func>
        TemplateDispatcher<dispatcher, Message, Func> handle(Func &&f)
        {
            return TemplateDispatcher<dispatcher, Message, Func>(q, this, std::forward<Func>(f));
        }

        ~dispatcher() noexcept(false)
        {
            if (!chained)
                wait_and_dispatch();
        }
    };
}

int main()
{

    return 0;
}
