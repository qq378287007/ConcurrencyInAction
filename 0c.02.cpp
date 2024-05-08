
#include <queue>
#include <memory>
using namespace std;

namespace messaging
{
    template <typename Message>
    class sender
    {
        shared_ptr<queue<Message>> q;

    public:
        explicit sender(shared_ptr<queue<Message>> q_ = nullptr) : q(q_) {}
        void send(Message const &msg)
        {
            if (q)
                q->push(msg);
        }
    };
}

using namespace messaging;
int main()
{
    // sender<int> s;
    sender<int> s(make_shared<queue<int>>());

    s.send(1);
    s.send(4);

    return 0;
}
