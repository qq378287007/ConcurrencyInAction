#include <memory>
using namespace std;

class no_copies
{
public:
    no_copies() = default;

    no_copies(no_copies const &) = delete;
    no_copies &operator=(no_copies const &) = delete;
    /*
    private:
        no_copies(no_copies const &);
        no_copies &operator=(no_copies const &);
    */
};

class my_class
{
};

class move_only
{
    unique_ptr<my_class> data;

public:
    move_only() = default;

    move_only(const move_only &) = delete;
    move_only &operator=(const move_only &) = delete;

    move_only(move_only &&other) : data(move(other.data)) {}
    move_only &operator=(move_only &&other)
    {
        data = move(other.data);
        return *this;
    }
};

int main()
{
    no_copies a;
    // no_copies b(a);//禁止拷贝

    no_copies c;
    // c = a; //禁止赋值

    move_only m1;
    //move_only m2(m1);//禁止拷贝
    move_only m3(move(m1));

    move_only m4;
    //m4 = m3;//禁止赋值
    m4 = move(m3);

    return 0;
}
