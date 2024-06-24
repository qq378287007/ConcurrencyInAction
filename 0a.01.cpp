#include <memory>
#include <string>
#include <cassert>
#include <vector>
using namespace std;

void printS(string const &s)
{
}

// 左值和右值皆可，都会强制复制
void process_copy1(vector<int> const &vec_)
{
    vector<int> vec(vec_);
    vec.push_back(42);
}

// 接收右值引用，避免复制（非const左值引用也行）
void process_copy2(vector<int> &&vec_)
{
    vec_.push_back(42);
}
class X
{
    int *data{nullptr};

public:
    X() : data(new int[1000000]) {}
    ~X() { delete[] data; }
    X(const X &other) : data(new int[1000000])
    {
        if (other.data != nullptr)
            copy(other.data, other.data + 1000000, data);
    }
    X(X &&other) : data(other.data) // 移动构造函数
    {
        other.data = nullptr;
    }
};

void do_stuff(X &&x)
{
    X a(x);       // 复制构造
    X b(move(x)); // 移动构造
}

// 万能引用
template <typename T>
void foo(T &&t)
{
}

int main()
{
    {
        int var = 42;
        int &ref = var;
        ref = 99;
        assert(var == 99);

        // int&i=42;error
        int const &ii = 42; // 右值绑定到const左值引用上

        printS("hello"); // 参数发生隐式转换，创建string类型的临时变量

        int &&i = 42;
        int j = 42;
        // int &&k = j;//error
    }

    // 移出，转换成右值
    X x1;
    X x2 = move(x1);
    X x3 = static_cast<X &&>(x2); // 同上

    
    // X x; do_stuff(x);//error,x左值，不能与右值引用绑定
    do_stuff(move(x1));
    do_stuff(X());//生成匿名临时对象，作为右值与右值引用绑定

    foo(3.14);     // foo<double>(3.14)
    foo(string()); // foo<string>(string())
    int i = 42;
    foo(i); // foo<int&>(i)
    return 0;
}
