/*!
 *  \author <https://github.com/chaos0x8>
 *  \copyright
 *  Copyright (c) 2015 - 2016, <https://github.com/chaos0x8>
 *
 *  \copyright
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  \copyright
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <gmock/gmock.h>
#include <Common/Timer.hpp>
#include <Common/Exceptions/TimerError.hpp>
#include <Traits/IsCopyable.hpp>
#include <Traits/IsMoveable.hpp>

#include <thread>
#include <chrono>
#include <atomic>

namespace Common
{
namespace UT
{

using namespace testing;
using namespace std::literals;

class TimerTestSuite : public Test
{
public:
    static void SetUpTestCase()
    {
        initTimers(MAX_TIMERS);
    }

    static void TearDownTestCase()
    {
        cleanupTimers();
    }

    void incrementCount()
    {
        ++count;
    }

    template <class T1, class T2, class T3>
    void delayedExpectThat(T1&& actual, T2&& expected, T3&& timeout)
    {
        auto start = std::chrono::high_resolution_clock::now();

        while (actual != expected and start + timeout > std::chrono::high_resolution_clock::now())
            std::this_thread::yield();

        EXPECT_THAT(actual, Eq(expected));
    }

    template <class T1, class T2, class T3, class T4>
    void delayedExpectThat(T1&& actual, T2&& expected, T3&& timeout, T4&& delayBefore = 0s)
    {
        std::this_thread::sleep_for(delayBefore);
        delayedExpectThat(std::forward<T1>(actual), std::forward<T2>(expected), std::forward<T3>(timeout));
    }

    static constexpr size_t MAX_TIMERS = 16;

    std::atomic<uint32_t> count{0};
};

constexpr size_t TimerTestSuite::MAX_TIMERS;

#if (UT_MODE == 1)
TEST_F(TimerTestSuite, testTimerRingBufferIndexAssign)
{
    for (size_t i = 0; i < MAX_TIMERS; ++i)
    {
        auto t = startTimer(1s, &TimerTestSuite::incrementCount, this);
        EXPECT_THAT(t._contextId(), Eq(i));
    }

    auto t = startTimer(1s, &TimerTestSuite::incrementCount, this);
    EXPECT_THAT(t._contextId(), Eq(0u));

    delayedExpectThat(count, 1, 1125ms);
}

TEST_F(TimerTestSuite, testTimerRingBufferIndexOverflow)
{
    std::vector<Detail::Timer> t;

    for (size_t i = 0; i < MAX_TIMERS; ++i)
        t.push_back(startTimer(1s, &TimerTestSuite::incrementCount, this));

    ASSERT_THROW(startTimer(1s, &TimerTestSuite::incrementCount, this), Exceptions::TimerError);

    delayedExpectThat(count, MAX_TIMERS, 1125ms);
}

TEST_F(TimerTestSuite, testTimerRingBufferUsedAndLastReleasedIndexIsSkipped)
{
    std::vector<Detail::Timer> t;

    for (size_t i = 0; i < MAX_TIMERS; ++i)
        t.push_back(startTimer(1s, &TimerTestSuite::incrementCount, this));

    constexpr size_t indexA = 4;
    static_assert(indexA > 0 and indexA < MAX_TIMERS - 1, "inproper index");
    const auto indexAId = t[indexA]._contextId();
    t[indexA] = nullptr;

    constexpr size_t indexB = 2;
    static_assert(indexB > 0 and indexB < MAX_TIMERS - 1, "inproper index");
    t[indexB] = nullptr;

    auto tt = startTimer(1s, &TimerTestSuite::incrementCount, this);
    EXPECT_THAT(tt._contextId(), Eq(indexAId));

    ASSERT_THROW(startTimer(1s, &TimerTestSuite::incrementCount, this), Exceptions::TimerError);

    delayedExpectThat(count, MAX_TIMERS - 1, 1125ms);
}
#endif

TEST_F(TimerTestSuite, isNotCopyable)
{
    EXPECT_THAT(std::is_trivially_copyable<Detail::Timer>::value, Eq(false));
    EXPECT_THAT(Traits::IsCopyable<Detail::Timer>::value, Eq(false));
    EXPECT_THAT(Traits::IsMoveable<Detail::Timer>::value, Eq(true));
}

TEST_F(TimerTestSuite, testTimer)
{
    auto t1 = startTimer(1s, std::bind(&TimerTestSuite::incrementCount, this));
    auto t2 = startTimer(2s, &TimerTestSuite::incrementCount, this);
    auto t3 = startTimer(1s, &TimerTestSuite::incrementCount, this);

    delayedExpectThat(count, 0, 0s);
    delayedExpectThat(count, 2, 1125ms);
    delayedExpectThat(count, 3, 1125ms);
}

TEST_F(TimerTestSuite, testStopTimerWhenNullptrAssigned)
{
    auto t = startTimer(1s, &TimerTestSuite::incrementCount, this);

    t = nullptr;

    delayedExpectThat(count, 0, 0s, 1125ms);
}

TEST_F(TimerTestSuite, testStopTimerWhenDestructorCalled)
{
    startTimer(1s, &TimerTestSuite::incrementCount, this);

    delayedExpectThat(count, 0, 0s, 1125ms);
}

TEST_F(TimerTestSuite, testMove)
{
    auto t1a = startTimer(1s, &TimerTestSuite::incrementCount, this);
    auto t1b = std::move(t1a);

    auto t2a = startTimer(1s, &TimerTestSuite::incrementCount, this);
    auto t2b = startTimer(1s, &TimerTestSuite::incrementCount, this);
    t2b = std::move(t2a);

    delayedExpectThat(count, 2, 1125ms);
}

}
}
