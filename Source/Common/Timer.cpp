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
#include <time.h>
#include <signal.h>
#include <cassert>
#include <memory>
#include <atomic>

namespace Common
{
namespace Detail
{

struct TimerRingBuffer;

std::unique_ptr<TimerRingBuffer> ringBuffer { nullptr };

struct TimerContext
{
    ~TimerContext()
    {
        if (timerId != timer_t{})
            ::timer_delete(timerId);
    }

    TimerCallback callback { };
    timer_t timerId { };
};

struct TimerRingBuffer
{
    TimerRingBuffer(size_t maxTimers)
        : maxTimers(maxTimers),
          lastReleasedIndex(maxTimers)
    {
        buffer = new std::atomic<TimerContext*>[maxTimers]{};
    }

    ~TimerRingBuffer()
    {
        for (size_t i = 0; i < maxTimers; ++i)
            delete releaseContext(i);
        delete[] buffer;
    }

    void free(TimerContext* context, size_t contextId)
    {
        lastReleasedIndex = contextId;

        if (buffer[contextId].compare_exchange_strong(context, nullptr))
            delete context;
    }

    std::tuple<TimerContext*, size_t> allocate(const std::chrono::seconds& timeout, TimerCallback& callback)
    {
        const size_t index = aquireIndex();

        TimerContext* oldPtr = { nullptr };
        TimerContext* const ptr = new TimerContext;

        if (buffer[index].compare_exchange_strong(oldPtr, ptr))
        {
            setupTimer(ptr, index, timeout, callback);
            return std::make_tuple(ptr, index);
        }
        else
        {
            delete ptr;
            throw Exceptions::TimerError("Attempted to allocate already taken buffer id");
        }
    }

    TimerContext* releaseContext(size_t index)
    {
        TimerContext* ptr = { nullptr };
        if (not buffer[index].compare_exchange_strong(ptr, nullptr))
        {
            lastReleasedIndex = index;

            if (buffer[index].compare_exchange_strong(ptr, nullptr))
                return ptr;
        }
        return nullptr;
    }

private:
    void setupTimer(TimerContext* context, size_t index, const std::chrono::seconds& timeout, TimerCallback& callback) try
    {
        context->callback = std::move(callback);

        sigevent sigEvent{};
        sigEvent.sigev_notify = SIGEV_THREAD;
        sigEvent.sigev_notify_function = &TimerRingBuffer::timerProcedure;
        sigEvent.sigev_value.sival_int = static_cast<int>(index);

        if (::timer_create(CLOCK_REALTIME, &sigEvent, &context->timerId) != 0)
            throw Exceptions::TimerError(errno);

        itimerspec timeSpec{};
        timeSpec.it_value.tv_sec = timeout.count();

        if (::timer_settime(context->timerId, 0, &timeSpec, nullptr) != 0)
            throw Exceptions::TimerError(errno);
    }
    catch (const Exceptions::TimerError&)
    {
        free(context, index);
        throw;
    }

    size_t aquireIndex()
    {
        size_t aquired = nextIndex.fetch_add(1);

        for (size_t attempt = 0; attempt < maxTimers - 1; ++attempt)
        {
            while (aquired >= maxTimers)
            {
                nextIndex.compare_exchange_strong(aquired += 1, 0u);
                aquired = nextIndex.fetch_add(1);
            }

            if (buffer[aquired] == nullptr and aquired != lastReleasedIndex)
                return aquired;

            aquired = nextIndex.fetch_add(1);
        }

        throw Exceptions::TimerError("No more free context ids");
    }

    static void timerProcedure(sigval arg)
    {
        const size_t index = static_cast<size_t>(arg.sival_int);
        if (TimerContext* const context = ringBuffer->releaseContext(index))
        {
            try {
                context->callback();
            } catch (...) { }
            delete context;
        }
    }

    const size_t maxTimers;
    std::atomic<TimerContext*>* buffer;
    std::atomic<size_t> nextIndex { 0u };
    std::atomic<size_t> lastReleasedIndex;
};

Timer::Timer(std::chrono::seconds timeout, TimerCallback callback)
{
    using namespace std::chrono;

    assert(timeout.count() > 0);

    std::tie(context, contextId) = ringBuffer->allocate(timeout, callback);
}

Timer::~Timer()
{
    ringBuffer->free(context, contextId);
}

Timer& Timer::operator = (std::nullptr_t)
{
    ringBuffer->free(context, contextId);

    context = nullptr;

    return *this;
}

Timer& Timer::operator = (Timer&& other)
{
    ringBuffer->free(context, contextId);

    context = other.context;
    other.context = nullptr;

    contextId = other.contextId;

    return *this;
}

}

void initTimers(size_t maxTimers)
{
    assert(maxTimers > 1);
    Detail::ringBuffer = std::make_unique<Detail::TimerRingBuffer>(maxTimers);
}

void cleanupTimers()
{
    Detail::ringBuffer = nullptr;
}

Detail::Timer startTimer(std::chrono::seconds timeout, TimerCallback callback)
{
    return Detail::Timer(std::move(timeout), std::move(callback));
}

}
