#include <Common/Eql.hpp>
#include <gmock/gmock.h>

namespace Common
{
namespace Ut
{

using namespace testing;

TEST(EqlTestSuite, shouldFloatsBeEqual)
{
    ASSERT_THAT(eql(2.0f, 2.f), Eq(true));
    ASSERT_THAT(eql(4.0, 4.0), Eq(true));
}

TEST(EqlTestSuite, shouldFloatBeNotEqual)
{
    ASSERT_THAT(eql(2.0f, 2.1f), Eq(false));
    ASSERT_THAT(eql(3.0, 4.0), Eq(false));
}

}
}

