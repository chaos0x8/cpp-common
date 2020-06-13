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

#include <Parallel/ForEach.hpp>
#include <gmock/gmock.h>
#include <chrono>

namespace Common
{
namespace Parallel
{

using namespace testing;

class ForEachTestData
{
public:
    void operation()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        executed = true;
    }

    bool executed{false};
    int padding{0};
};

class ForEachTestSuite : public Test
{
public:
    void assertAllTestData(bool expectedValue) const
    {
        ASSERT_THAT(std::all_of(std::begin(testData), std::end(testData), [=] (const ForEachTestData& x) { return x.executed == expectedValue; }), Eq(true));
    }
    void assertAnyOfTestData(bool expectedValue) const
    {
        ASSERT_THAT(std::any_of(std::begin(testData), std::end(testData), [=] (const ForEachTestData& x) { return x.executed == expectedValue; }), Eq(true));
    }

    std::vector<ForEachTestData> testData{375};
};

TEST_F(ForEachTestSuite, shouldExecuteAll)
{
    forEach(testData, std::mem_fn(&ForEachTestData::operation), 24);
    assertAllTestData(true);
}

TEST_F(ForEachTestSuite, shouldHoldExecutionContext)
{
    auto context = forEach(testData, std::mem_fn(&ForEachTestData::operation));
    assertAnyOfTestData(false);
    context->wait();
    assertAnyOfTestData(true);
}

}
}