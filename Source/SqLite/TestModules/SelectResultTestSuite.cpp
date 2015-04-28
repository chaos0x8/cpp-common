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

#include <SqLite/SelectResult.hpp>
#include <gmock/gmock.h>
#include <boost/format.hpp>
#include <stdexcept>

namespace Common
{
namespace SqLite
{
namespace UT
{

using namespace testing;

class SelectResultTestSuite : public Test
{
public:
    void SetUp() override
    {
        sut.addColumn("id", 0);
        sut.addColumn("name", 1);
        sut.addColumn("value", 2);
    }

    SelectResult sut;
};

MATCHER_P4(CheckRow, index, id, name, value, boost::str(boost::format("expected { %1%, %2%, %3% }") % id % name % value))
{
    return arg.at(index, "id") == id
        && arg.at(index, "name") == name
        && arg.at(index, "value") == value;
}

TEST_F(SelectResultTestSuite, testEmptyResult)
{
    ASSERT_TRUE(sut.empty());
    ASSERT_THAT(sut, SizeIs(0u));
}

TEST_F(SelectResultTestSuite, testFewElements)
{
    sut.push_back({"1", "ted", "dal"});
    sut.push_back({"2", "kyra", "shadow"});
    sut.push_back({"3", "izzie", "lone"});

    ASSERT_FALSE(sut.empty());
    ASSERT_THAT(sut, SizeIs(3u));

    ASSERT_THAT(sut, CheckRow(0, "1", "ted", "dal"));
    ASSERT_THAT(sut, CheckRow(1, "2", "kyra", "shadow"));
    ASSERT_THAT(sut, CheckRow(2, "3", "izzie", "lone"));

    ASSERT_THROW(sut.at(0, "non-existing"), std::out_of_range);

    size_t id, value, name;
    std::tie(id, value, name) = sut.mapColumns("id", "value", "name");
    ASSERT_THAT(id, Eq(0));
    ASSERT_THAT(value, Eq(2));
    ASSERT_THAT(name, Eq(1));

    const std::vector<SelectResult::Row>& rows = sut.rows();
    ASSERT_THAT(rows[0][value], Eq("dal"));
    ASSERT_THAT(rows[1][name], Eq("kyra"));
}

}
}
}
