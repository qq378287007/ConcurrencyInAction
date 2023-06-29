#include <string>
#include <vector>
#include <future>
using namespace std;

template <>
class packaged_task<string(vector<char> *, int)>
{
public:
    template <typename Callable>
    explicit packaged_task(Callable &&f);
    
    future<string> get_future();
    void operator()(vector<char> *, int);
};

int main()
{
    return 0;
}
