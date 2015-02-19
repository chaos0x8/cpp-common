#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <SqLite/Row.hpp>

namespace Library
{
namespace SqLite
{

using namespace testing;

class RowTestSuite : public Test
{
public:
    Row cut;
};

TEST_F(RowTestSuite, shouldFindElement)
{
    cut.push_back(Item("column", "value"));
    cut.push_back(Item("col", "val"));

    ASSERT_THAT(cut["column"], Eq("value"));
    ASSERT_THAT(cut["col"], Eq("val"));
}

}
}
