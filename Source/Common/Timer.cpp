/*!
    \author <https://github.com/chaos0x8>
    \copyright
    Copyright (c) 2015, <https://github.com/chaos0x8>

    \copyright
    Permission to use, copy, modify, and/or distribute this software for any
    purpose with or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    \copyright
    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <Common/Timer.hpp>
#include <Common/Exceptions/TimerError.hpp>
#include <cassert>
#include <stdexcept>
#include <pthread.h>

namespace Common
{
namespace Detail
{

Timer::Timer(std::chrono::seconds timeout, TimerCallback callback)
    : callback{std::move(callback)}
{
    using namespace std::chrono;

    assert(timeout.count() > 0);

    sigevent sigEvent{};
    sigEvent.sigev_notify = SIGEV_THREAD;
    sigEvent.sigev_notify_function = &Timer::threadProcedure;
    sigEvent.sigev_value.sival_ptr = this;

    if (::timer_create(CLOCK_REALTIME, &sigEvent, &timerId) != 0)
        throw Exceptions::TimerError(errno);

    itimerspec timeSpec{};
    timeSpec.it_value.tv_sec = timeout.count();

    if (::timer_settime(timerId, 0, &timeSpec, nullptr) != 0)
        throw Exceptions::TimerError(errno);
}

Timer::~Timer()
{
    ::timer_delete(timerId);
}

void Timer::threadProcedure(sigval arg)
{
    Timer* _this = reinterpret_cast<Timer*>(arg.sival_ptr);
    TimerCallback callback = std::move(_this->callback);
    delete _this;

    callback();
}

}

void startTimer(std::chrono::seconds timeout, TimerCallback callback)
{
    new Detail::Timer(std::move(timeout), std::move(callback));
}

}
