#include <GetCurrentYear.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace Common
{
namespace UT
{

using namespace testing;

TEST(GetCurrentYearTestSuite, shouldGetCurrentYear)
{
    ASSERT_THAT(getCurrentYear(), Ge(2014u));
}

}
}
