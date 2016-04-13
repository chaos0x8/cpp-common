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

#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <SqLite/Item.hpp>

namespace Common
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
