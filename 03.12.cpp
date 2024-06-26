#include <mutex>
using namespace std;

struct connection_info
{
};

struct data_packet
{
};

struct connection_handle
{
    void send_data(data_packet const &) {}
    data_packet receive_data() { return data_packet(); }
};

connection_handle open(connection_info const &) { return connection_handle(); }

class X
{
    connection_info connection_details;
    connection_handle connection;
    once_flag connection_init_flag;

    void open_connection() { connection = open(connection_details); }

public:
    X(connection_info const &connection_details_) : connection_details(connection_details_) {}
    void send_data(data_packet const &data)
    {
        call_once(connection_init_flag, &X::open_connection, this);
        connection.send_data(data);
    }
    data_packet receive_data()
    {
        call_once(connection_init_flag, &X::open_connection, this);
        return connection.receive_data();
    }
};

int main()
{
    X x{connection_info()};
    x.receive_data();
    x.send_data(data_packet());

    return 0;
}
