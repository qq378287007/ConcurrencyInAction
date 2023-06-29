#include <mutex>
using namespace std;

class Y
{
private:
    int some_detail;
    mutable mutex m;

    int get_detail() const
    {
        lock_guard<mutex> lock_a(m);
        return some_detail;
    }

public:
    Y(int sd) : some_detail(sd) {}

    friend bool operator==(Y const &lhs, Y const &rhs)
    {
        if (&lhs == &rhs)
            return true;
            
        int const lhs_value = lhs.get_detail();
        int const rhs_value = rhs.get_detail();
        return lhs_value == rhs_value;
    }
};

int main()
{
    return 0;
}
