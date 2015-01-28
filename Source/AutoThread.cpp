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
    _join();
}

AutoThread& AutoThread::operator = (AutoThread&& other) noexcept
{
    this->_join();
    this->t = std::move(other.t);
    return *this;
}

AutoThread& AutoThread::operator = (std::thread t)
{
    this->_join();
    this->t = std::move(t);
    return *this;
}

void AutoThread::_join()
{
    if (t.joinable())
        t.join();
}

}
