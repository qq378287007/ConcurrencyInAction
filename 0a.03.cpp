class CX
{
    int a;
    int b;

public:
    CX() = default;
    CX(int a_, int b_) : a(a_), b(b_) {}
    int get_a() const { return a; }
    int get_b() const { return b; }
    int foo() const { return a + b; }
};

class Y
{
public:
    Y(Y &) = default; // 接受非const引用
    Y &operator=(const Y &) = default;

protected:
    virtual ~Y() = default;

private:
    Y() = default; // 改变访问级别
};

struct aggregate
{
    aggregate() = default;
    aggregate(aggregate const &) = default;
    int a;
    double b;
};

template <unsigned size>
struct test
{
};

int main()
{
    // 常量表达式
    const int i = 23;
    const int two_i = i * 2;
    const int four = 4;
    const int forty_two = two_i - four;

    int bounds = 99;
    // int array[bounds];//error
    // test<bounds> ia;//error

    const int bounds2 = 99;
    int array2[bounds2];
    test<bounds2> ia2;

    aggregate x = {42, 3.14};

    return 0;
}
