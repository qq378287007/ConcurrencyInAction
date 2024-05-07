#include <future>
using namespace std;

struct payload_type
{
};

struct data_packet
{
    int id;
    payload_type payload;
};

struct outgoing_packet
{
    payload_type payload;
    promise<payload_type> promise;
};

class connection_set
{
    int m_Value;
    int m_Length;
    promise<payload_type> p;

public:
    int value() const { return m_Value; }
    void setValue(int Value) { m_Value = Value; }
    int length() const { return m_Length; }
    void setLength(int Length) { m_Length = Length; }

    bool has_incoming_data() const { return true; }
    bool has_outgoing_data() const { return true; }
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
        void operator++() { ++pos; }
        void operator++(int i) { pos++; }
        bool operator!=(const connection_iterator &it) { return it.pos != pos; }
        int operator*() const { return 0; }
        connection_set *operator->() { return obj; }
    };

    connection_iterator begin() { return connection_iterator(); }
    connection_iterator end() { return connection_iterator(); }
};

bool done(connection_set &connections) { return false; }

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
                data.promise.set_value(data.payload);
            }
        }
    }
}

int main()
{
    return 0;
}
