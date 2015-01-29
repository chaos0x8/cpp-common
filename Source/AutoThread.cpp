#include <AutoThread.hpp>

namespace Common
{

AutoThread::AutoThread(std::thread t)
    : t(std::move(t))
{
}

AutoThread::AutoThread(AutoThread&& other) noexcept
    : t(std::move(other.t))
{
}

AutoThread::~AutoThread()
{
    join();
}

AutoThread& AutoThread::operator = (AutoThread&& other) noexcept
{
    this->join();
    this->t = std::move(other.t);
    return *this;
}

AutoThread& AutoThread::operator = (std::thread t)
{
    this->join();
    this->t = std::move(t);
    return *this;
}

void AutoThread::join()
{
    if (t.joinable())
        t.join();
}

}
