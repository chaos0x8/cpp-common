#include <Common/TypeTraces.hpp>
#include <gmock/gmock.h>
#include <vector>

namespace Common
{
namespace TypeTraces
{
namespace UT
{

using namespace testing;

struct TypeTracesTestSuite : public Test
{
  int carray[3];
  std::vector<int> vector;
};

TEST_F(TypeTracesTestSuite, returnsTrueWhenCArrayUsed)
{
  ASSERT_THAT(IsCArray<decltype(carray)>::value, Eq(true));
}

TEST_F(TypeTracesTestSuite, returnsFalseWhenVectorUsed)
{
  ASSERT_THAT(IsCArray<decltype(vector)>::value, Eq(false));
}

}
}
}
