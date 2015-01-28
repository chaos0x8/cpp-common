#pragma once

#include <thread>

namespace Common
{

class AutoThread
{
public:
    AutoThread() = default;
    AutoThread(std::thread);
    AutoThread(const AutoThread&) = delete;
    AutoThread(AutoThread&&) noexcept;
    ~AutoThread();

    AutoThread& operator = (const AutoThread&) = delete;
    AutoThread& operator = (AutoThread&& other) noexcept;
    AutoThread& operator = (std::thread t);

private:
    void _join();

    std::thread t;
};

}
