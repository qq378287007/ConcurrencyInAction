#include <string>
#include <future>
using namespace std;

struct X
{
    void foo(int, string const &) const {}
    string bar(string const &str) const { return str; }
};

struct Y
{
    double operator()(double data) const { return data; }
};

struct move_only
{
    move_only() {}
    move_only(move_only &&) {}
    move_only(move_only const &) = delete;
    move_only &operator=(move_only &&) { return *this; }
    move_only &operator=(move_only const &) = delete;
    void operator()() const {}
};

X baz(const X &x)
{
    return x;
}

int main()
{
    X x;
    auto f1 = async(&X::foo, &x, 42, "hello");
    auto f2 = async(&X::bar, x, "goodbye");

    auto f3 = async(Y(), 3.141);
    Y y;
    auto f4 = async(ref(y), 2.718);

    auto f5 = async(move_only());

    auto f6 = async(baz, ref(x));

    return 0;
}
