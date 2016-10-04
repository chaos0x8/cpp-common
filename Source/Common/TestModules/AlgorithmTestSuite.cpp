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
#include <list>
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

  auto eq(int v)
  {
    return [v](auto x) { return x == v; };
  }

  auto ge(int v)
  {
    return [v](auto x) { return x >= v; };
  }
};

struct AlgorithmTestSuite_includes : public AlgorithmTestSuite
  { };

TEST_F(AlgorithmTestSuite_includes, returnsTrueWhenIncludesElement)
{
  EXPECT_THAT(includes(data, 3), Eq(true));
  EXPECT_THAT(includes(data, eq(3)), Eq(true));
}

TEST_F(AlgorithmTestSuite_includes, returnsFalseWhenDoesntIncludeElement)
{
  EXPECT_THAT(includes(data, 42), Eq(false));
  EXPECT_THAT(includes(data, eq(42)), Eq(false));
}

struct AlgorithmTestSuite_any : public AlgorithmTestSuite
{
  auto functor(int v)
  {
    return [v](auto x) { return x % v == 0; };
  }
};

TEST_F(AlgorithmTestSuite_any, returnsTrueWhenOneOfElementsFullfilsCondition)
{
  EXPECT_THAT(any(data, functor(2)), Eq(true));
}

TEST_F(AlgorithmTestSuite_any, returnsFalseWhenNoneOfElementsFullfilsCondition)
{
  EXPECT_THAT(any(data, functor(10)), Eq(false));
}

struct AlgorithmTestSuite_transform : public AlgorithmTestSuite
{
  auto functor()
  {
    return [](auto& x) { return std::to_string(x); };
  }
};

TEST_F(AlgorithmTestSuite_transform, returnsDifferentTypes)
{
  const std::vector<std::string> res = transform(data, functor());
  EXPECT_THAT(res, ElementsAre("1", "3", "4"));
}

TEST_F(AlgorithmTestSuite_transform, returnsDifferentTypesWithSpecifiesContainer)
{
  const std::list<std::string> res = transform<std::list>(data, functor());
  EXPECT_THAT(res, ElementsAre("1", "3", "4"));
}

struct AlgorithmTestSuite_filter : public AlgorithmTestSuite
{
  auto predicate()
  {
    return [](auto& x) { return x == 1 or x == 4; };
  }
};

TEST_F(AlgorithmTestSuite_filter, returnsDifferentTypes)
{
  const std::vector<int> res = filter(data, predicate());
  EXPECT_THAT(res, ElementsAre(1, 4));
}

TEST_F(AlgorithmTestSuite_filter, returnsDifferentTypesWithSpecifiesContainer)
{
  const std::list<int> res = filter<std::list>(data, predicate());
  EXPECT_THAT(res, ElementsAre(1, 4));
}

struct AlgorithmTestSuite_first : public AlgorithmTestSuite
  { };

TEST_F(AlgorithmTestSuite_first, returnsFirstElementWhichFullfilsCondition)
{
  auto res = first(data, ge(2));

  ASSERT_THAT(res, Not(Eq(std::end(data))));
  ASSERT_THAT(*res, Eq(3));
}

TEST_F(AlgorithmTestSuite_first, returnsEndIteratorWhenElementIsNotFound)
{
  auto res = first(data, ge(5));

  ASSERT_THAT(res, Eq(std::end(data)));
}

struct AlgorithmTestSuite_count : public AlgorithmTestSuite
    { };

TEST_F(AlgorithmTestSuite_count, returnsNumberOfElements)
{
  ASSERT_THAT(count(data, 3), Eq(1));
  ASSERT_THAT(count(data, 5), Eq(0));

  ASSERT_THAT(count(data, ge(2)), Eq(2));
  ASSERT_THAT(count(data, ge(1)), Eq(3));
}

}
}
}
