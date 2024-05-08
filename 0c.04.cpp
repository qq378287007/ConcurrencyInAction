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
            while (q.empty())
                c.wait(lk);

            shared_ptr<message_base> res = q.front();
            q.pop();
            return res;
        }
    };
}

namespace messaging
{
    template <typename PreviousDispatcher, typename Msg, typename Func>
    class TemplateDispatcher
    {
        queueMy *q;
        PreviousDispatcher *prev;
        Func f;
        bool chained;

        TemplateDispatcher(TemplateDispatcher const &) = delete;
        TemplateDispatcher &operator=(TemplateDispatcher const &) = delete;

        template <typename Dispatcher, typename OtherMsg, typename OtherFunc>

        friend class TemplateDispatcher;

        void wait_and_dispatch()
        {
            while (true)
            {
                auto msg = q->wait_and_pop();
                if (dispatch(msg))
                    break;
            }
        }

        bool dispatch(shared_ptr<message_base> const &msg)
        {
            if (wrapped_message<Msg> *wrapper = dynamic_cast<wrapped_message<Msg> *>(msg.get()))
            {
                f(wrapper->contents);
                return true;
            }
            else
            {
                return prev->dispatch(msg);
            }
        }

    public:
        TemplateDispatcher(TemplateDispatcher &&other)
            : q(other.q), prev(other.prev), f(move(other.f)), chained(other.chained)
        {
            other.chained = true;
        }

        TemplateDispatcher(queueMy *q_, PreviousDispatcher *prev_, Func &&f_)
            : q(q_), prev(prev_), f(forward<Func>(f_)), chained(false)
        {
            prev_->chained = true;
        }

        template <typename OtherMsg, typename OtherFunc>
        TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc> handle(OtherFunc &&of)
        {
            return TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc>(q, this, forward<OtherFunc>(of));
        }

        ~TemplateDispatcher() noexcept(false)
        {
            if (!chained)
                wait_and_dispatch();
        }
    };
}

namespace messaging
{
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
            while (true)
            {
                auto msg = q->wait_and_pop();

                dispatch(msg);
            }
        }

        bool dispatch(shared_ptr<message_base> const &msg)
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
            return TemplateDispatcher<dispatcher, Message, Func>(q, this, forward<Func>(f));
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
