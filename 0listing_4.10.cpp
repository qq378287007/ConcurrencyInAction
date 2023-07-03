#include <future>
using namespace std;

using payload_type = int;

struct data_packet
{
    int id;
    int payload;
};

template <typename T>
class promise
{
    T p;
public:
    promise(){};
    void set_value(int) {}
};

/*
template <>
class promise<payload_type>
{
public:
    void set_value(int) {}
};
*/

struct outgoing_packet
{
    payload_type payload;
    promise<payload_type> promise;
};

class connection_set
{
    int val;    // 具体的数字
    int length; // 数字的位数
    promise<payload_type> p;
    void getlength()
    {
        if (val / 10 == 0)
        { // 这个数字只有1位
            length = 1;
            return;
        }
        int x = 10; // 这里就是不断重复除直到为0，从而得出数字的具体位数
        int pow = 0;
        int num = val;
        while (num != 0)
        {
            num /= 10;
            pow++;
        }
        length = pow;
    }

public:
    void num(int num)
    { // 以下是一些基本的函数，用于设置值
        val = num;
        getlength();
    }
    void set(int num)
    {
        val = num;
        getlength();
    }
    int get()
    {
        return val;
    }

    bool has_incoming_data() { return true; }
    bool has_outgoing_data() { return true; }
    outgoing_packet top_of_outgoing_queue() { return outgoing_packet(); }

    data_packet incoming() { return data_packet(); }

    promise<payload_type> &get_promise(int) { return p; }

    void send(payload_type) {}

    class connection_iterator
    {
        int pos;
        connection_set *obj;

    public:
        connection_iterator()
        {
            pos = 0;
            obj = nullptr;
        }
        void operator++()
        {
            ++pos;
        }
        void operator++(int i)
        {
            pos++;
        }

        bool operator!=(const connection_iterator &it)
        {
            return it.pos != pos;
        }

        int operator*() const
        {
            return 0;
        }

        connection_set *operator->()
        {
            return obj;
        }
    };

    connection_iterator begin() { return connection_iterator(); }
    connection_iterator end() { return connection_iterator(); }
};

bool done(connection_set &connections)
{
    return true;
}

void process_connections(connection_set &connections)
{
    while (!done(connections))
    {
        for (connection_set::connection_iterator connection = connections.begin(), end = connections.end(); connection != end; ++connection)
        {
            if (connection->has_incoming_data())
            {
                data_packet data = connection->incoming();
                promise<payload_type> &p = connection->get_promise(data.id);
                p.set_value(data.payload);
            }
            if (connection->has_outgoing_data())
            {
                outgoing_packet data = connection->top_of_outgoing_queue();
                connection->send(data.payload);
                data.promise.set_value(true);
            }
        }
    }
}

int main()
{
    return 0;
}