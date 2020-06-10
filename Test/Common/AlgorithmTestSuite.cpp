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
#include <gmock/gmock.h>
#include <list>
#include <vector>

namespace Common {
  namespace Algorithm {
    namespace UT {

      using namespace testing;

      struct AlgorithmTestSuite : public Test {
        const std::vector<int> data = {1, 3, 4};
        std::vector<int> rw_data = data;
        int raw_data[3] = {1, 3, 4};

        auto eq(int v) {
          return [v](auto x) { return x == v; };
        }

        auto ge(int v) {
          return [v](auto x) { return x >= v; };
        }

        auto rsort() {
          return [](auto l, auto r) { return l > r; };
        }

        auto div(int v) {
          return [v](auto x) { return x % v == 0; };
        }
      };

      struct AlgorithmTestSuite_all : public AlgorithmTestSuite {};

      TEST_F(
        AlgorithmTestSuite_all, returnsTrueWhenAllOfElementsFullfilsCondition) {
        EXPECT_THAT(all(data, ge(1)), Eq(true));
        EXPECT_THAT(all(raw_data, ge(1)), Eq(true));
      }

      TEST_F(AlgorithmTestSuite_all,
        returnsFalseWhenAnyOfElementsDoesntFullfilCondition) {
        EXPECT_THAT(all(data, ge(2)), Eq(false));
        EXPECT_THAT(all(raw_data, ge(2)), Eq(false));
      }

      struct AlgorithmTestSuite_any : public AlgorithmTestSuite {};

      TEST_F(
        AlgorithmTestSuite_any, returnsTrueWhenOneOfElementsFullfilsCondition) {
        EXPECT_THAT(any(data, div(2)), Eq(true));
        EXPECT_THAT(any(raw_data, div(2)), Eq(true));
      }

      TEST_F(AlgorithmTestSuite_any,
        returnsFalseWhenNoneOfElementsFullfilsCondition) {
        EXPECT_THAT(any(data, div(10)), Eq(false));
        EXPECT_THAT(any(raw_data, div(10)), Eq(false));
      }

      struct AlgorithmTestSuite_includes : public AlgorithmTestSuite {};

      TEST_F(AlgorithmTestSuite_includes, returnsTrueWhenIncludesElement) {
        EXPECT_THAT(includes(data, 3), Eq(true));
        EXPECT_THAT(includes(raw_data, 3), Eq(true));
        EXPECT_THAT(includes(data, eq(3)), Eq(true));
        EXPECT_THAT(includes(raw_data, eq(3)), Eq(true));
      }

      TEST_F(
        AlgorithmTestSuite_includes, returnsFalseWhenDoesntIncludeElement) {
        EXPECT_THAT(includes(data, 42), Eq(false));
        EXPECT_THAT(includes(raw_data, 42), Eq(false));
        EXPECT_THAT(includes(data, eq(42)), Eq(false));
        EXPECT_THAT(includes(raw_data, eq(42)), Eq(false));
      }

      struct AlgorithmTestSuite_transform : public AlgorithmTestSuite {
        auto functor() {
          return [](auto& x) { return std::to_string(x); };
        }
      };

      TEST_F(AlgorithmTestSuite_transform, returnsDifferentTypes) {
        const std::vector<std::string> res = transform(data, functor());
        EXPECT_THAT(res, ElementsAre("1", "3", "4"));
      }

      TEST_F(AlgorithmTestSuite_transform,
        returnsDifferentTypesWithSpecifiesContainer) {
        const std::list<std::string> res =
          transform<std::list>(data, functor());
        EXPECT_THAT(res, ElementsAre("1", "3", "4"));
      }

      TEST_F(AlgorithmTestSuite_transform, returnsDifferentTypes_withRawData) {
        const std::vector<std::string> res = transform(raw_data, functor());
        EXPECT_THAT(res, ElementsAre("1", "3", "4"));
      }

      TEST_F(AlgorithmTestSuite_transform,
        returnsDifferentTypesWithSpecifiesContainer_withRawData) {
        const std::list<std::string> res =
          transform<std::list>(raw_data, functor());
        EXPECT_THAT(res, ElementsAre("1", "3", "4"));
      }

      struct AlgorithmTestSuite_filter : public AlgorithmTestSuite {
        auto predicate() {
          return [](auto& x) { return x == 1 or x == 4; };
        }
      };

      TEST_F(AlgorithmTestSuite_filter, returnsDifferentTypes) {
        const std::vector<int> res = filter(data, predicate());
        EXPECT_THAT(res, ElementsAre(1, 4));
      }

      TEST_F(AlgorithmTestSuite_filter,
        returnsDifferentTypesWithSpecifiesContainer) {
        const std::list<int> res = filter<std::list>(data, predicate());
        EXPECT_THAT(res, ElementsAre(1, 4));
      }

      TEST_F(AlgorithmTestSuite_filter, returnsDifferentTypes_withRawData) {
        const std::vector<int> res = filter(raw_data, predicate());
        EXPECT_THAT(res, ElementsAre(1, 4));
      }

      TEST_F(AlgorithmTestSuite_filter,
        returnsDifferentTypesWithSpecifiesContainer_withRawData) {
        const std::list<int> res = filter<std::list>(raw_data, predicate());
        EXPECT_THAT(res, ElementsAre(1, 4));
      }

      TEST_F(AlgorithmTestSuite_filter, worksWithContainerWithoutRandomAccess) {
        auto set_data = std::set<int>(std::begin(data), std::end(data));
        const std::set<int> res = filter<std::set>(set_data, predicate());
        EXPECT_THAT(res, ElementsAre(1, 4));
      }

      struct AlgorithmTestSuite_first : public AlgorithmTestSuite {};

      TEST_F(
        AlgorithmTestSuite_first, returnsFirstElementWhichFullfilsCondition) {
        auto res = first(data, ge(2));

        ASSERT_THAT(*res, Eq(3));
      }

      TEST_F(AlgorithmTestSuite_first, returnsNulloptWhenElementIsNotFound) {
        auto res = first(data, ge(5));

        ASSERT_THAT(res, Eq(std::nullopt));
      }

      TEST_F(AlgorithmTestSuite_first,
        returnsFirstElementWhichFullfilsCondition_withRawData) {
        auto res = first(raw_data, ge(2));

        ASSERT_THAT(*res, Eq(3));
      }

      TEST_F(AlgorithmTestSuite_first,
        returnsNulloptWhenElementIsNotFound_withRawData) {
        auto res = first(raw_data, ge(5));

        ASSERT_THAT(res, Eq(std::nullopt));
      }

      struct AlgorithmTestSuite_count : public AlgorithmTestSuite {};

      TEST_F(AlgorithmTestSuite_count, returnsNumberOfElements) {
        ASSERT_THAT(count(data, 3), Eq(1));
        ASSERT_THAT(count(raw_data, 3), Eq(1));
        ASSERT_THAT(count(data, 5), Eq(0));
        ASSERT_THAT(count(raw_data, 5), Eq(0));

        ASSERT_THAT(count(data, ge(2)), Eq(2));
        ASSERT_THAT(count(raw_data, ge(2)), Eq(2));
        ASSERT_THAT(count(data, ge(1)), Eq(3));
        ASSERT_THAT(count(raw_data, ge(1)), Eq(3));
      }

      struct AlgorithmTestSuite_sort : public AlgorithmTestSuite {};

      TEST_F(AlgorithmTestSuite_sort, sortElements) {
        sort(rw_data, rsort());
        ASSERT_THAT(rw_data, ElementsAre(4, 3, 1));

        sort(rw_data);
        ASSERT_THAT(rw_data, ElementsAre(1, 3, 4));
      }

      TEST_F(AlgorithmTestSuite_sort, sortElements_withRawData) {
        sort(raw_data, rsort());
        ASSERT_THAT(raw_data, ElementsAre(4, 3, 1));

        sort(raw_data);
        ASSERT_THAT(raw_data, ElementsAre(1, 3, 4));
      }

      struct AlgorithmTestSuite_max : public AlgorithmTestSuite {};

      TEST_F(AlgorithmTestSuite_max, returnsMaxElementFromContainer) {
        ASSERT_THAT(max(data), Eq(4));
        ASSERT_THAT(max(raw_data), Eq(4));
        ASSERT_THAT(max(data, rsort()), Eq(1));
        ASSERT_THAT(max(raw_data, rsort()), Eq(1));
      }

      TEST_F(AlgorithmTestSuite_max, returnsNulloptWhenInputContainerIsEmpty) {
        ASSERT_THAT(max(std::vector<int>{}), Eq(std::nullopt));
      }

      struct AlgorithmTestSuite_erase : public AlgorithmTestSuite {};

      TEST_F(AlgorithmTestSuite_erase, removesElementFromContainerUsingValue) {
        Common::Algorithm::erase(rw_data, 3);
        ASSERT_THAT(rw_data, UnorderedElementsAre(1, 4));
      }

      TEST_F(
        AlgorithmTestSuite_erase, removesElementFromContainerUsingFunctor) {
        Common::Algorithm::erase(rw_data, ge(3));
        ASSERT_THAT(rw_data, UnorderedElementsAre(1));
      }

      struct AlgorithmTestSuite_forEach : public AlgorithmTestSuite {
        struct Foo {
          int x = 0;
          int y = 0;

          void bar() {
            x = 42;
          }
        };

        void SetUp() override {
          uniqFooPtrData.emplace_back(std::make_unique<Foo>());
          uniqFooPtrData.emplace_back(std::make_unique<Foo>());
          uniqFooPtrData.emplace_back(std::make_unique<Foo>());
        }

        void TearDown() override {
          for (auto* ptr : fooPtrData)
            delete ptr;
        }

        auto modY(int v) {
          return [v](auto& foo) { foo.y = v; };
        }

        std::vector<Foo> fooData{3};
        std::vector<Foo*> fooPtrData{new Foo{3}, new Foo{3}, new Foo{3}};
        std::vector<std::unique_ptr<Foo>> uniqFooPtrData;
      };

      TEST_F(AlgorithmTestSuite_forEach, executesStructMethod) {
        Common::Algorithm::forEach(fooData, &Foo::bar);
        ASSERT_THAT(fooData, ElementsAre(Field(&Foo::x, Eq(42)),
                               Field(&Foo::x, Eq(42)), Field(&Foo::x, Eq(42))));
      }

      TEST_F(AlgorithmTestSuite_forEach, executesStructMethodFromPointer) {
        Common::Algorithm::forEach(fooPtrData, &Foo::bar);
        ASSERT_THAT(
          fooPtrData, ElementsAre(Field(&Foo::x, Eq(42)),
                        Field(&Foo::x, Eq(42)), Field(&Foo::x, Eq(42))));
      }

      TEST_F(AlgorithmTestSuite_forEach, executesStructMethodFromSmartPointer) {
        Common::Algorithm::forEach(uniqFooPtrData, &Foo::bar);
        ASSERT_THAT(uniqFooPtrData, SizeIs(3));
        EXPECT_THAT(uniqFooPtrData[0]->x, Eq(42));
        EXPECT_THAT(uniqFooPtrData[1]->x, Eq(42));
        EXPECT_THAT(uniqFooPtrData[2]->x, Eq(42));
      }

      TEST_F(AlgorithmTestSuite_forEach, executesLambda) {
        Common::Algorithm::forEach(fooData, modY(13));
        ASSERT_THAT(fooData, ElementsAre(Field(&Foo::y, Eq(13)),
                               Field(&Foo::y, Eq(13)), Field(&Foo::y, Eq(13))));
      }

    } // namespace UT
  }   // namespace Algorithm
} // namespace Common
