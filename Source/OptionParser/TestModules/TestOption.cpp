/*!
 *  \author <https://github.com/chaos0x8>
 *  \copyright
 *  Copyright (c) 2017, <https://github.com/chaos0x8>
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

#include "../Option.hpp"
#include <gmock/gmock.h>
#include <optional>

namespace Common::OptionParser
{
  using namespace testing;

  struct Size
  {
    Size() = default;
    explicit Size(const std::string& text)
      : _value(text.size())
    {
    }

    size_t value() const
    {
      return _value;
    }

  private:
    size_t _value = 0u;
  };

  TEST(TestOption, shouldAssignDescription)
  {
    auto sut = Option<std::string>("-n", "--name").description("desc");
    EXPECT_THAT(sut.description(), Eq("desc"));
  }

  TEST(TestOption, shouldBeNotValidWhenValueIsNotSet)
  {
    auto sut = Option<std::string>("-n", "--name");
    EXPECT_THAT(static_cast<bool>(sut), Eq(false));
  }

  TEST(TestOption, shouldDoNothingWhenSetUsedOnNonBoolOption)
  {
    auto sut = Option<std::string>("-n", "--name").set();
    EXPECT_THAT(static_cast<bool>(sut), Eq(false));
    EXPECT_THAT(sut.value(), Eq(std::string()));
  }

  TEST(TestOption, shouldBeValidWhenValueIsNotSetButParamHasDefault)
  {
    auto sut = Option<std::string>("-n", "--name").defaultValue("def");
    EXPECT_THAT(static_cast<bool>(sut), Eq(true));
    EXPECT_THAT(sut.value(), Eq("def"));
  }

  TEST(TestOption, shouldAssignValue)
  {
    auto sut = Option<std::string>("-n", "--name").value("val");
    EXPECT_THAT(static_cast<bool>(sut), Eq(true));
    EXPECT_THAT(sut.value(), Eq("val"));
  }

  TEST(TestOption, shouldAutoConvertToInteger)
  {
    auto sut = Option<int>("--number").value("42");
    EXPECT_THAT(static_cast<bool>(sut), Eq(true));
    EXPECT_THAT(sut.value(), Eq(42));
  }

  TEST(TestOption, shouldAutoConvertToDouble)
  {
    auto sut = Option<double>("--number").value("42.7");
    EXPECT_THAT(static_cast<bool>(sut), Eq(true));
    EXPECT_THAT(sut.value(), DoubleEq(42.7));
  }

  TEST(TestOption, shouldBeSetByDefaultToFalseOnBoolOption)
  {
    auto sut = Option<bool>("--help");
    EXPECT_THAT(static_cast<bool>(sut), Eq(false));
    EXPECT_THAT(sut.value(), Eq(false));
  }

  TEST(TestOption, shouldEvaluateToValueOnBoolOption)
  {
    auto sut = Option<bool>("--help").value("false");
    EXPECT_THAT(static_cast<bool>(sut), Eq(false));
    EXPECT_THAT(sut.value(), Eq(false));

    sut = Option<bool>("--help").value("true");
    EXPECT_THAT(static_cast<bool>(sut), Eq(true));
    EXPECT_THAT(sut.value(), Eq(true));
  }

  TEST(TestOption, shouldAutoConvertToBool)
  {
    auto sut = Option<bool>("--help").value("true");
    EXPECT_THAT(static_cast<bool>(sut), Eq(true));
    EXPECT_THAT(sut.value(), Eq(true));
  }

  TEST(TestOption, shouldAssignTrueWhenSetUsedOnBoolOption)
  {
    auto sut = Option<bool>("--help").set();
    EXPECT_THAT(static_cast<bool>(sut), Eq(true));
    EXPECT_THAT(sut.value(), Eq(true));
  }

  TEST(TestOption, shouldAutoConvertToClass)
  {
    auto sut = Option<Size>("-s", "--size").value("42");
    EXPECT_THAT(static_cast<bool>(sut), Eq(true));
    EXPECT_THAT(sut.value().value(), Eq(2u));
  }

  TEST(TestOption, shouldExecuteCallbackWhenValueIsSet)
  {
    std::optional<std::string> actual;

    auto sut = Option<std::string>("--name").on([&](const std::string& val){
      actual = val;
    });

    sut.value("Jessy");

    EXPECT_THAT(actual, Eq("Jessy"));
  }
}
