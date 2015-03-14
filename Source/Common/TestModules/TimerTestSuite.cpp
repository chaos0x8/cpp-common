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

#include <gmock/gmock.h>
#include <Common/Timer.hpp>

#include <thread>
#include <chrono>
#include <atomic>

namespace Common
{
namespace UT
{

using namespace testing;

class TimerTestSuite : public Test
{
public:
    void incrementCount()
    {
        ++count;
    }

    std::atomic<uint32_t> count{0};
};

TEST_F(TimerTestSuite, testTimer)
{
    startTimer(std::chrono::seconds(1), std::bind(&TimerTestSuite::incrementCount, this));
    startTimer(std::chrono::seconds(2), &TimerTestSuite::incrementCount, this);
    startTimer(std::chrono::seconds(1), &TimerTestSuite::incrementCount, this);

    ASSERT_THAT(count, Eq(0));

    std::this_thread::sleep_for(std::chrono::milliseconds(1005));

    ASSERT_THAT(count, Eq(2));

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    ASSERT_THAT(count, Eq(2));

    while (count != 3)
        std::this_thread::yield();

    ASSERT_THAT(count, Eq(3));
}

}
}
