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

#include <Parallel/Compute.hpp>
#include <gmock/gmock.h>
#include <chrono>

namespace Common
{
namespace Parallel
{

using namespace testing;

class ComputeTestData
{
public:
    int operation()
    {
        return 42;
    }

    int padding{0};
};

class ComputeTestSuite : public Test
{
public:
    void assertAllTestData(const std::vector<int>& data, int expectedValue) const
    {
        ASSERT_THAT(std::all_of(std::begin(data), std::end(data), [=] (const int& x) { return x == expectedValue; }), Eq(true));
    }

    std::vector<ComputeTestData> testData{375};
};

TEST_F(ComputeTestSuite, shouldExecuteAll)
{
    auto result = compute(testData, std::mem_fn(&ComputeTestData::operation), 24)->get();
    assertAllTestData(result, 42);
}

TEST_F(ComputeTestSuite, shouldHoldExecutionContext)
{
    auto context = compute(testData, std::mem_fn(&ComputeTestData::operation));
    context->wait();
    assertAllTestData(context->get(), 42);
}

}
}
