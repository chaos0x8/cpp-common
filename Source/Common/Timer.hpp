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

#include <functional>
#include <cstddef>
#include <chrono>

namespace Common
{

using TimerCallback = std::function<void ()>;

namespace Detail
{

struct TimerContext;

struct Timer
{
public:
    //! \throw Exceptions::TimerError
    Timer(std::chrono::seconds timeout, TimerCallback callback);
    Timer(Timer&& other)
        : context(other.context),
          contextId(other.contextId)
    {
        other.context = nullptr;
    }

    ~Timer();

    Timer& operator = (std::nullptr_t);
    Timer& operator = (Timer&& other);

    #if (UT_MODE == 1)
    size_t _contextId() const { return contextId; }
    #endif

private:
    TimerContext* context { nullptr };
    size_t contextId { 0u };
};

}

void initTimers(size_t maxTimers = 32);
void cleanupTimers();

//! \throw Exceptions::TimerError
Detail::Timer startTimer(std::chrono::seconds timeout, TimerCallback);

//! \throw Exceptions::TimerError
template <class F, class... Args>
Detail::Timer startTimer(std::chrono::seconds timeout, F&& f, Args&&... args)
{
    return Detail::Timer(std::move(timeout), std::bind(std::forward<F>(f), std::forward<Args>(args)...));
}

}
