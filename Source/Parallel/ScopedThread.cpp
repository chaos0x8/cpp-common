#include <Parallel/ScopedThread.hpp>

namespace Common
{
namespace Parallel
{

ScopedThread::ScopedThread(std::thread t)
    : t(std::move(t))
{
}

ScopedThread::ScopedThread(ScopedThread&& other) noexcept
    : t(std::move(other.t))
{
}

ScopedThread::~ScopedThread()
{
    join();
}

ScopedThread& ScopedThread::operator = (ScopedThread&& other) noexcept
{
    this->join();
    this->t = std::move(other.t);
    return *this;
}

ScopedThread& ScopedThread::operator = (std::thread t)
{
    this->join();
    this->t = std::move(t);
    return *this;
}

void ScopedThread::join()
{
    if (t.joinable())
        t.join();
}

}
}
