#include <map>
#include <string>
#include <mutex>
#include <shared_mutex>
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
        map<string, dns_entry>::const_iterator const it =
            entries.find(domain);
        return (it == entries.end()) ? dns_entry() : it->second;
    }
    void update_or_add_entry(string const &domain,
                             dns_entry const &dns_details)
    {
        lock_guard<shared_mutex> lk(entry_mutex);
        entries[domain] = dns_details;
    }
};

int main()
{
    return 0;
}

// g++ listing_3.13.cpp -std=c++17 -o listing_3.13 && listing_3.13
