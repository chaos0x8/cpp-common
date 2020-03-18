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

#include <Parallel/ScopedThread.hpp>
#include <gmock/gmock.h>
#include <atomic>
#include <chrono>

namespace Common
{
namespace Parallel
{
namespace UT
{

using namespace testing;

class TestObject
{
public:
    void operation()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        count++;
    }

    std::atomic<uint32_t> count{0};
};

class ScopedThreadTestSuite : public Test
{
public:
    void TearDown() override
    {
        ASSERT_THAT(obj.count, Eq(expectedCount));
    }

    TestObject obj;
    uint32_t expectedCount{0};
};

TEST_F(ScopedThreadTestSuite, itsSaveToNotJoin)
{
    expectedCount = 1;

    ScopedThread t(std::mem_fn(&TestObject::operation), &obj);
}

TEST_F(ScopedThreadTestSuite, doubleJoinIsPossible)
{
    expectedCount = 1;

    ScopedThread t1(std::mem_fn(&TestObject::operation), &obj);
    t1.join();
    t1.join();
}

TEST_F(ScopedThreadTestSuite, easyToStoreInContainers)
{
    expectedCount = 5;

    std::vector<ScopedThread> threads;
    for (size_t i = 0; i < expectedCount; ++i)
        threads.emplace_back(std::mem_fn(&TestObject::operation), &obj);
}

}
}
}
