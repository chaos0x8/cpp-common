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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <SqLite/DBCast.hpp>
#include <boost/optional.hpp>

namespace Common
{
namespace SqLite
{
namespace DBCast
{
namespace UT
{

using namespace testing;

TEST(DBCastTestSuite, shouldCastToDbFormat)
{
    ASSERT_THAT(toDBFormat(15), Eq("15"));
    ASSERT_THAT(toDBFormat(15.5), Eq("15.5"));
    ASSERT_THAT(toDBFormat("15"), Eq("'15'"));
    ASSERT_THAT(toDBFormat(boost::optional<int>(15)), Eq("15"));
    ASSERT_THAT(toDBFormat(boost::optional<std::string>("15")), Eq("'15'"));
    ASSERT_THAT(toDBFormat(boost::optional<std::string>()), Eq("null"));
    ASSERT_THAT(toDBFormat("Hello'World"), Eq("'Hello''World'"));
}

TEST(DBCastTestSuite, shouldCastFromDbFormat)
{
    ASSERT_THAT(fromDBFormat<int>("15"), Eq(15));
    ASSERT_THAT(fromDBFormat<double>("15.5"), Eq(15.5));
    ASSERT_THAT(fromDBFormat<std::string>("15"), Eq("15"));
    ASSERT_THAT(fromDBFormat<boost::optional<int>>("15"), Eq(15));
    ASSERT_THAT(fromDBFormat<boost::optional<std::string>>("15"), Eq(std::string("15")));
    ASSERT_THAT(fromDBFormat<boost::optional<std::string>>(std::string()), Eq(boost::none));
}

}
}
}
}
