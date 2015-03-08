#pragma once

#include <thread>

namespace Common
{
namespace Parallel
{

class ScopedThread
{
public:
    template <class... Args>
    explicit ScopedThread(Args&&... args)
        : t(std::forward<Args>(args)...)
    {
    }
    ScopedThread(std::thread);
    ScopedThread(const ScopedThread&) = delete;
    ScopedThread(ScopedThread&&) noexcept;
    ~ScopedThread();

    ScopedThread& operator = (const ScopedThread&) = delete;
    ScopedThread& operator = (ScopedThread&& other) noexcept;
    ScopedThread& operator = (std::thread t);

    void join();

private:
    std::thread t;
};

}
}
