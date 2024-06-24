#include <condition_variable>
#include <mutex>
using namespace std;

// 常量表达式
const int i = 23;
const int two_i = i * 2;
const int four = 4;
const int forty_two = two_i - four;

template <unsigned size>
struct test
{
};

class X
{
    static const int the_answer = forty_two; // 静态常量整型数据成员的初始化表达式
};

struct my_aggregate
{
    int a;
    int b;
};
static my_aggregate ma1 = {forty_two, 123}; // 静态初始化
int dummy = 257;
static my_aggregate ma2 = {dummy, dummy}; // 动态初始化

constexpr int square(int x)
{
    return x * x;
}
int array[square(5)];
// int array2[square(dummy)];//error，dummy非常量表达式

class CX
{
    int a;
    int b;

public:
    CX() = default;
    constexpr CX(int a_, int b_) : a(a_), b(b_) {}

    constexpr int get_a() const { return a; }
    constexpr int get_b() { return b; }
    constexpr int foo() const { return a + b; }
};

constexpr CX create_cx()
{
    return CX();
}
constexpr CX clone(CX val)
{
    return val;
}

constexpr CX make_cx(int a)
{
    return CX(a, 1);
}
constexpr CX half_double(CX old)
{
    return CX(old.get_a() / 2, old.get_b() * 2);
}
constexpr int foo_squared(CX val)
{
    return square(val.foo());
}
int array3[foo_squared(half_double(make_cx(10)))];

CX si = half_double(CX(42, 19)); // 静态初始化

int foo() { return 11; }

template <typename T>
constexpr T sum(T a, T b)
{
    return a + b;
}

int main()
{
    int bounds = 99;
    // int array[bounds];//error

    const int bounds2 = 99;
    int array2[bounds2];

    // test<bounds> ia;//error
    test<bounds2> ia2;

    constexpr int i = 45;
    // constexpr string s("hello");//error, string非字面值型别
    // constexpr int j = foo();//error，foo()非constexpr函数

    constexpr int ii = sum(3, 42);                    // sum<int>具备constexpr特性
    string s = sum(string("hello"), string("world")); // sum<string> 不具备constexpr特性 return 0;
}
