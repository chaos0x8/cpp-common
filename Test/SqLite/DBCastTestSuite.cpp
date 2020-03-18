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
using namespace std::literals;

TEST(DBCastTestSuite, shouldCastToDbFormat)
{
    ASSERT_THAT(toDBFormat(15), Eq("15"));
    ASSERT_THAT(toDBFormat(15.5), Eq("15.5"));
    ASSERT_THAT(toDBFormat("15"), Eq("'15'"));
    ASSERT_THAT(toDBFormat(boost::optional<int>(15)), Eq("15"));
    ASSERT_THAT(toDBFormat(boost::optional<std::string>("15")), Eq("'15'"));
    ASSERT_THAT(toDBFormat(boost::optional<std::string>()), Eq("null"));
    ASSERT_THAT(toDBFormat("Hello'World"), Eq("'Hello''World'"));
    ASSERT_THAT(toDBFormat(boost::optional<double>(12.7)), Eq("12.7"));
}

TEST(DBCastTestSuite, shouldCastFromDbFormat)
{
    ASSERT_THAT(fromDBFormat<uint32_t>("15"), Eq(15));
    ASSERT_THAT(fromDBFormat<uint64_t>("15"), Eq(15));
    ASSERT_THAT(fromDBFormat<int>("15"), Eq(15));
    ASSERT_THAT(fromDBFormat<double>("15.5"), Eq(15.5));
    ASSERT_THAT(fromDBFormat<std::string>("15"), Eq("15"));
    ASSERT_THAT(fromDBFormat<boost::optional<int>>("15"), Eq(15));
    ASSERT_THAT(fromDBFormat<boost::optional<std::string>>("15"), Eq(std::string("15")));
    ASSERT_THAT(fromDBFormat<boost::optional<std::string>>(std::string()), Eq(boost::none));
    ASSERT_DOUBLE_EQ(12.7, fromDBFormat<boost::optional<double>>("12.7").get_value_or(0.));
}

TEST(DBCastTestSuite, shouldCastFromDbFormat_String) {
    ASSERT_THAT(fromDBFormat<uint32_t>("15s"), Eq(15));
    ASSERT_THAT(fromDBFormat<uint64_t>("15s"), Eq(15));
    ASSERT_THAT(fromDBFormat<int>("15"s), Eq(15));
    ASSERT_THAT(fromDBFormat<double>("15.5"s), Eq(15.5));
    ASSERT_THAT(fromDBFormat<std::string>("15"s), Eq("15"));
    ASSERT_THAT(fromDBFormat<boost::optional<int>>("15"s), Eq(15));
    ASSERT_THAT(fromDBFormat<boost::optional<std::string>>("15"s), Eq(std::string("15")));
    ASSERT_THAT(fromDBFormat<boost::optional<std::string>>(std::string()), Eq(boost::none));
    ASSERT_DOUBLE_EQ(12.7, fromDBFormat<boost::optional<double>>("12.7"s).get_value_or(0.));
}

TEST(DBCastTestSuite, integralCastFromDbFormat)
{
    ASSERT_TRUE(fromDBFormat<bool>("1"));
    ASSERT_FALSE(fromDBFormat<bool>("0"));
    ASSERT_EQ('s', fromDBFormat<char>("s"));
    ASSERT_EQ(-42, fromDBFormat<int>("-42"));
    ASSERT_EQ(-42, fromDBFormat<long>("-42"));
    ASSERT_EQ(-42, fromDBFormat<long long>("-42"));
    ASSERT_EQ(42, fromDBFormat<unsigned int>("42"));
    ASSERT_EQ(42, fromDBFormat<unsigned long>("42"));
    ASSERT_EQ(42, fromDBFormat<unsigned long long>("42"));
    ASSERT_FLOAT_EQ(42.2f, fromDBFormat<float>("42.2"));
    ASSERT_DOUBLE_EQ(42.2, fromDBFormat<double>("42.2"));
    ASSERT_DOUBLE_EQ(-42.2, fromDBFormat<long double>("-42.2"));
}

}
}
}
}
