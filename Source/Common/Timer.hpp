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

#pragma once

#include <time.h>
#include <signal.h>
#include <functional>
#include <chrono>
#include <memory>

namespace Common
{

using TimerCallback = std::function<void ()>;

class Timer
{
    class TimerData
    {
    public:
        //! \throw Exceptions::TimerError
        TimerData(std::chrono::seconds sec, TimerCallback callback);
        TimerData(const TimerData&) = delete;

        ~TimerData();

        TimerData& operator = (const TimerData&) = delete;

        static void threadProcedure(sigval arg);

        void deleteTimer();

    private:
        TimerCallback callback;
        timer_t timerId{};
    };

public:
    Timer() = default;
    Timer(Timer&&) noexcept = default;

    //! \throw Exceptions::TimerError
    Timer(std::chrono::seconds sec, TimerCallback callback);

    //! \throw Exceptions::TimerError
    template <class F, class... Args>
    Timer(std::chrono::seconds sec, F&& f, Args&&... args)
        : timerData{new TimerData{
            std::move(sec),
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)}}
    {
    }

    Timer& operator = (Timer&&) noexcept = default;

private:
    std::unique_ptr<TimerData> timerData{nullptr};
};

}
