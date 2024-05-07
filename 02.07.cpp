#include <thread>
#include <iostream>
using namespace std;

// noexcept
// 编译期完成声明和检查工作
// 减少运行时开销(额外代码，异常等错误信息)
// 若报错, 进程直接结束, 不会抛出异常信息.
class joining_thread
{
    thread t;

public:
    joining_thread() noexcept = default;

    template <typename Callable, typename... Args>
    explicit joining_thread(Callable &&func, Args &&...args) : t(forward<Callable>(func), forward<Args>(args)...)
    {
    }

    explicit joining_thread(thread t_) noexcept : t(move(t_))
    {
        cout << "joining_thread\n";
    }

    joining_thread(joining_thread &&other) noexcept : t(move(other.t))
    {
    }

    joining_thread &operator=(joining_thread &&other) noexcept
    {
        if (joinable())
            join();

        t = move(other.t);
        return *this;
    }

    joining_thread &operator=(thread other) noexcept
    {
        if (joinable())
            join();

        t = move(other);
        return *this;
    }

    ~joining_thread() noexcept
    {
        if (joinable())
            join();

        cout << "~joining_thread\n";
    }

    void swap(joining_thread &other) noexcept { t.swap(other.t); }
    thread::id get_id() const noexcept { return t.get_id(); }

    bool joinable() const noexcept { return t.joinable(); }
    void join() noexcept { t.join(); }
    void detach() noexcept { t.detach(); }

    thread &as_thread() noexcept { return t; }
    const thread &as_thread() const noexcept { return t; }
};

void func()
{
    cout << "func!\n";
}

int main()
{
    {
        joining_thread jt{thread(func)};
    }

    cout << "Over!\n";
    return 0;
}
