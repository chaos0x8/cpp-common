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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <SqLite/Item.hpp>
#include <SqLite/TestModules/Matchers/Item.hpp>
#include <SqLite/TestModules/Printers/Item.hpp>

namespace Common
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
