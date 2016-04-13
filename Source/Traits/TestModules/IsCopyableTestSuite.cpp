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

#include <Traits/IsCopyable.hpp>
#include <gmock/gmock.h>
#include <memory>
#include <thread>

namespace Common
{
namespace Traits
{

using namespace testing;

TEST(IsCopyableTestSuite, isCopyableWhenHaveCopyConstructor)
{
    EXPECT_THAT(IsCopyable<int>::value, Eq(true));
    EXPECT_THAT(IsCopyable<std::string>::value, Eq(true));
    EXPECT_THAT(IsCopyable<std::shared_ptr<int>>::value, Eq(true));
}

TEST(IsCopyableTestSuite, isCopyableWhenCopyConstructorIsDisabled)
{
    EXPECT_THAT(IsCopyable<std::unique_ptr<int>>::value, Eq(false));
    EXPECT_THAT(IsCopyable<std::thread>::value, Eq(false));
}

}
}
