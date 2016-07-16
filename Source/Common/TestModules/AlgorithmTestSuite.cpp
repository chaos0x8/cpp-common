/*!
 *  \author <https://github.com/chaos0x8>
 *  \copyright
 *  Copyright (c) 2016, <https://github.com/chaos0x8>
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

#include <Common/Algorithm.hpp>
#include <vector>
#include <gmock/gmock.h>

namespace Common
{
namespace Algorithm
{
namespace UT
{

using namespace testing;

struct AlgorithmTestSuite : public Test
{
  const std::vector<int> data = { 1, 3, 4 };
};

struct AlgorithmTestSuite_includes : public AlgorithmTestSuite
  { };

TEST_F(AlgorithmTestSuite_includes, returnsTrueWhenIncludesElement)
{
  EXPECT_THAT(includes(data, 3), Eq(true));
  EXPECT_THAT(includes_if(data, [](auto x) { return x == 3; }), Eq(true));
}

TEST_F(AlgorithmTestSuite_includes, returnsFalseWhenDoesntIncludeElement)
{
  EXPECT_THAT(includes(data, 42), Eq(false));
  EXPECT_THAT(includes_if(data, [](auto x) { return x == 42; }), Eq(false));
}

struct AlgorithmTestSuite_any : public AlgorithmTestSuite
  { };

TEST_F(AlgorithmTestSuite_any, returnsTrueWhenOneOfElementsFullfilsCondition)
{
  EXPECT_THAT(any(data, [](auto x) { return x % 2 == 0; }), Eq(true));
}

TEST_F(AlgorithmTestSuite_any, returnsFalseWhenNoneOfElementsFullfilsCondition)
{
  EXPECT_THAT(any(data, [](auto x) { return x % 10 == 0; }), Eq(false));
}

}
}
}
