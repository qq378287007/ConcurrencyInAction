
#include <queue>
#include <memory>
using namespace std;

namespace messaging
{
    template <typename Message>
    class sender
    {
    public:
        explicit sender(shared_ptr<queue<Message>> q_ = nullptr) : q(q_)
        {
        }
        void send(Message const &msg)
        {
            if (q)
                q->push(msg);
        }

    private:
        shared_ptr<queue<Message>> q;
    };
}

int main()
{
    return 0;
}
