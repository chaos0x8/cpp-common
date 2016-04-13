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

#include <GL/Utility.hpp>
#include <gmock/gmock.h>

namespace Common
{
namespace GL
{
namespace Ut
{

using namespace testing;

TEST(UtilityTestSuite, testToSeconds)
{
    using namespace std::chrono;

    ASSERT_THAT(toSeconds(milliseconds(500)), FloatEq(0.5f));
    ASSERT_THAT(toSeconds(milliseconds(1700)), FloatEq(1.7f));
    ASSERT_THAT(toSeconds(milliseconds(234)), FloatEq(0.234f));
}

}
}
}
