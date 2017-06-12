#include "../Option.hpp"
#include <gmock/gmock.h>

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

  TEST(TestOption, shouldAssignName)
  {
    auto sut = Option<std::string>("name");
    EXPECT_THAT(sut.name(), Eq("name"));
    EXPECT_THAT(sut.info(), Eq("\tname"));
  }

  TEST(TestOption, shouldAssignDescription)
  {
    auto sut = Option<std::string>("name").description("desc");
    EXPECT_THAT(sut.description(), Eq("desc"));
    EXPECT_THAT(sut.info(), Eq("\tname\tdesc"));
  }

  TEST(TestOption, shouldBeNotValidWhenValueIsNotSet)
  {
    auto sut = Option<std::string>("name");
    EXPECT_THAT(static_cast<bool>(sut), Eq(false));
  }

  TEST(TestOption, shouldDoNothingWhenSetUsedOnNonBoolOption)
  {
    auto sut = Option<std::string>("name").set();
    EXPECT_THAT(static_cast<bool>(sut), Eq(false));
    EXPECT_THAT(sut.value(), Eq(std::string()));
  }

  TEST(TestOption, shouldBeValidWhenValueIsNotSetButParamHasDefault)
  {
    auto sut = Option<std::string>("name").defaultValue("def");
    EXPECT_THAT(static_cast<bool>(sut), Eq(true));
    EXPECT_THAT(sut.value(), Eq("def"));
    EXPECT_THAT(sut.info(), Eq("\tname\n\t\tdefault: def"));
  }

  TEST(TestOption, shouldAssignValue)
  {
    auto sut = Option<std::string>("name").value("val");
    EXPECT_THAT(static_cast<bool>(sut), Eq(true));
    EXPECT_THAT(sut.value(), Eq("val"));
  }

  TEST(TestOption, shouldAutoConvertToInteger)
  {
    auto sut = Option<int>("name").value("42");
    EXPECT_THAT(static_cast<bool>(sut), Eq(true));
    EXPECT_THAT(sut.value(), Eq(42));
  }

  TEST(TestOption, shouldBeSetByDefaultToFalseOnBoolOption)
  {
    auto sut = Option<bool>("name");
    EXPECT_THAT(static_cast<bool>(sut), Eq(true));
    EXPECT_THAT(sut.value(), Eq(false));
    EXPECT_THAT(sut.info(), Eq("\tname\n\t\tdefault: false"));
  }

  TEST(TestOption, shouldAutoConvertToBool)
  {
    auto sut = Option<bool>("name").value("true");
    EXPECT_THAT(static_cast<bool>(sut), Eq(true));
    EXPECT_THAT(sut.value(), Eq(true));
  }

  TEST(TestOption, shouldAssignTrueWhenSetUsedOnBoolOption)
  {
    auto sut = Option<bool>("name").set();
    EXPECT_THAT(static_cast<bool>(sut), Eq(true));
    EXPECT_THAT(sut.value(), Eq(true));
  }

  TEST(TestOption, shouldAutoConvertToClass)
  {
    auto sut = Option<Size>("name").value("42");
    EXPECT_THAT(static_cast<bool>(sut), Eq(true));
    EXPECT_THAT(sut.value().value(), Eq(2u));
  }
}
