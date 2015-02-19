#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <SqLite/DBCast.hpp>
#include <boost/optional.hpp>

namespace Library
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
