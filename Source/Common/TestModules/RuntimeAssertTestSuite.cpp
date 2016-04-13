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

#include <Common/RuntimeAssert.hpp>
#include <Common/Exceptions/AssertionFailed.hpp>
#include <gmock/gmock.h>

namespace Common
{
namespace UT
{

using namespace testing;

class RuntimeAssertTestSuite : public Test
{
public:
    const std::string TEXT = "assert message";
};

TEST_F(RuntimeAssertTestSuite, doesNothingWhenExpressionIsTrue)
{
    ASSERT_NO_THROW(runtimeAssert(true, TEXT));
}

TEST_F(RuntimeAssertTestSuite, throwsWhenExpressionIsFalse)
{
    try
    {
        runtimeAssert(false, TEXT);
        FAIL() << "Exception was not thrown!";
    }
    catch (Exceptions::AssertionFailed& e)
    {
        using S = std::string;
        ASSERT_THAT(S{e.what()}, Eq(S{Exceptions::AssertionFailed(TEXT).what()}));
    }
}

}
}
