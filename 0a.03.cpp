class Y
{
public:
    Y(Y &) = default; // 接受非const引用
    Y &operator=(const Y &) = default;

protected:
    virtual ~Y() = default; // 加入虚函数性质

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
struct X
{
    int a;
};

int main()
{

    aggregate x = {42, 3.14};

    X x1;       // x1.a值未知
    X x2 = X(); // x2.a为0
    // X::X():a(){}//a为0
    // X::X(){}//a未知

    return 0;
}
