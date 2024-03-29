#include <memory>
#include <string>

class X
{
public:
    X() : data(new int[1000000])
    {
    }
    ~X()
    {
        delete[] data;
    }
    X(const X &other) : data(new int[1000000])
    {
        std::copy(other.data, other.data + 1000000, data);
    }
    X(X &&other) : data(other.data)
    {
        other.data = nullptr;
    }

private:
    int *data{nullptr};
};

void do_stuff(X &&x)
{
    X a(x);            // 复制构造
    X b(std::move(x)); // 移动构造
}

// 万能引用
template <typename T>
void foo(T &&t)
{
}

int main()
{
    X x1;
    X x2 = std::move(x1);
    X x3 = static_cast<X &&>(x2); // 同上

    // do_stuff(x1);//error
    do_stuff(std::move(x1));
    do_stuff(X());

    foo(3.14);          // foo<double>(3.14)
    foo(std::string()); // foo<std::string>(std::string())
    int i = 42;
    foo(i); // foo<int&>(i)

    return 0;
}
