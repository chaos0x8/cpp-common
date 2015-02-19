#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <SqLite/Item.hpp>

namespace Library
{
namespace SqLite
{
namespace UT
{

using namespace testing;

class EqItemMatcher : public MatcherInterface<Item>
{
public:
    explicit EqItemMatcher(const std::string& column, const std::string& value)
        : column(column),
          value(value)
    {
    }

    bool MatchAndExplain(Item x, MatchResultListener*) const
    {
        return this->column == x.column
            && this->value == x.value;
    }

    void DescribeTo(std::ostream* os) const
    {
        (*os) << "Item is equal to { " << column << ", " << value << " }";
    }

    void DescribeNegationTo(std::ostream* os) const
    {
        (*os) << "Item is not equal to { " << column << ", " << value << " }";
    }

private:
    std::string column;
    std::string value;
};

inline Matcher<Item> Eq(const std::string& col, const std::string& val)
{
    return MakeMatcher(new EqItemMatcher(col, val));
}

}
}
}
