#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <SqLite/Item.hpp>
#include <SqLite/TestModules/Matchers/Item.hpp>
#include <SqLite/TestModules/Printers/Item.hpp>

namespace Library
{
namespace SqLite
{
namespace UT
{

using namespace testing;

class ItemTestSuite : public Test
{
};

TEST(ItemTestSuite, shouldCreateItem)
{
    ASSERT_THAT(Item("col", "val"), Eq("col", "val"));
    ASSERT_THAT(Item("c", ""), Eq("c", ""));
    ASSERT_THAT(Item(0, 0), Eq("", ""));
}

}
}
}
