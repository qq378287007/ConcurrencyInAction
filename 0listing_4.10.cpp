#include <future>
using namespace std;

class connection_set
{
};

bool done(connection_set &connections){
    return true;
}

void process_connections(connection_set &connections)
{
    while (!done(connections))
    {
        for (connection_iterator
                 connection = connections.begin(),
                 end = connections.end();
             connection != end;
             ++connection)
        {
            if (connection->has_incoming_data())
            {
                data_packet data = connection->incoming();
                promise<payload_type> &p =
                    connection->get_promise(data.id);
                p.set_value(data.payload);
            }
            if (connection->has_outgoing_data())
            {
                outgoing_packet data =
                    connection->top_of_outgoing_queue();
                connection->send(data.payload);
                data.promise.set_value(true);
            }
        }
    }
}

int main(){
    return 0;
}