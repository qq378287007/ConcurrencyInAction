#include <map>
#include <string>
#include <mutex>
#include <shared_mutex>
#include <thread>
using namespace std;

class dns_entry
{
};

class dns_cache
{
    map<string, dns_entry> entries;
    shared_mutex entry_mutex;

public:
    dns_entry find_entry(string const &domain)
    {
        shared_lock<shared_mutex> lk(entry_mutex);
        map<string, dns_entry>::const_iterator const it = entries.find(domain);
        return (it == entries.cend()) ? dns_entry() : it->second;
    }
    void update_or_add_entry(string const &domain, dns_entry const &dns_details)
    {
        lock_guard<shared_mutex> lk(entry_mutex);
        entries[domain] = dns_details;
    }
};

int main()
{
    dns_cache dns;

    thread t1(
        [&dns]
        {
            for (int i = 0; i < 10; i++)
                dns.update_or_add_entry(to_string(i), dns_entry());
        });

    thread t2(
        [&dns]
        {
            for (int i = 0; i < 10; i++)
                dns.find_entry("5");
        });

    t1.join();
    t2.join();

    return 0;
}
