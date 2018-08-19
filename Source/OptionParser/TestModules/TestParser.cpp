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

#include "../Parser.hpp"
#include "../Option.hpp"
#include "../Tagged.hpp"
#include "ArgumentsBuilder.hpp"
#include <gmock/gmock.h>
#include <cstring>
#include <memory>

namespace Common::OptionParser
{
  using namespace testing;
  using namespace std::literals;

  enum class Tag : uint32_t
  {
    Name,
    Number,
    Help,
    Custom,
    Stuff1,
    Stuff2
  };

  struct Custom
  {
    Custom() = default;
    Custom(std::string val)
      : _val(val)
    {
    }

    friend std::ostream& operator << (std::ostream& os, const Custom& o)
    {
      os << o._val;
      return os;
    }

  private:
    std::string _val;
  };

  auto makeSut()
  {
    auto res = makeParser<Tag>(
        tagged<Tag, Tag::Name>(Option<std::string>("-n", "--name").description("some name")),
        tagged<Tag, Tag::Number>(Option<int>("--number").description("some number").defaultValue("17")),
        tagged<Tag, Tag::Help>(Option<bool>("-h", "--help").description("some flag")),
        tagged<Tag, Tag::Custom>(Option<Custom>("--custom")),
        tagged<Tag, Tag::Stuff1>(Option<bool>("--[no-]stuff1").defaultValue("true")),
        tagged<Tag, Tag::Stuff2>(Option<bool>("--[no-]stuff2").defaultValue("false")));
    return res;
  }

  struct TestParser : public Test
  {
    void setArguments(std::shared_ptr<ArgumentsBuilder> arguments)
    {
      _arguments = arguments;
      std::tie(argc, argv) = _arguments->view();
    }

    int argc;
    char** argv;

  private:
    std::shared_ptr<ArgumentsBuilder> _arguments;
  };

  struct TestParser_A : public TestParser,
                        public WithParamInterface<std::shared_ptr<ArgumentsBuilder>>
  {
    void SetUp() override
    {
      setArguments(GetParam());
    }
  };

  INSTANTIATE_TEST_CASE_P(, TestParser_A, Values(
    makeArgs("-nfdsfdf", "--help", "--number", "42"),
    makeArgs("-n", "fdsfdf", "--help", "--number", "42"),
    makeArgs("-hnfdsfdf", "--number", "42"),
    makeArgs("--name", "fdsfdf", "--help", "--number", "42"),
    makeArgs("--name=fdsfdf", "--help", "--number=42")));

  TEST_P(TestParser_A, shouldAssignValueToOptions)
  {
    auto sut = makeSut();
    sut.parse(&argc, argv);

    EXPECT_THAT(sut.get<Tag::Name>().value(), Eq("fdsfdf"));
    EXPECT_THAT(sut.get<Tag::Number>().value(), Eq(42));
    EXPECT_THAT(sut.get<Tag::Help>().value(), Eq(true));
  }

  TEST_F(TestParser, shouldThowWhenBooleanOptionAppearedWithValue)
  {
    setArguments(makeArgs("--help=true"));

    auto sut = makeSut();

    ASSERT_THROW(sut.parse(&argc, argv), UnexpectedValueError);
  }

  struct TestParser_B : public TestParser,
                        public WithParamInterface<std::shared_ptr<ArgumentsBuilder>>
  {
    void SetUp() override
    {
      setArguments(GetParam());
    }
  };

  INSTANTIATE_TEST_CASE_P(, TestParser_B, Values(
    makeArgs("--name=", "fdsfdf"),
    makeArgs("--name")));

  TEST_P(TestParser_B, shouldThrowWhenNonBooleanOptionAppearedWithoutValue)
  {
    auto sut = makeSut();

    ASSERT_THROW(sut.parse(&argc, argv), MissingValueError);
  }

  TEST_F(TestParser, shouldDisplayHelp)
  {
    auto sut = makeSut();

    sut.addHelpPrefix("prefix");
    sut.addHelpPrefix("--");

    sut.addHelpSufix("--");
    sut.addHelpSufix("sufix");

    EXPECT_THAT(sut.help(), Eq(
      "prefix\n"
      "--\n"
      "  -n, --name           some name\n"
      "  --number             some number\n"
      "                         default: 17\n"
      "  -h, --help           some flag\n"
      "  --custom\n"
      "  --[no-]stuff1        default: true\n"
      "  --[no-]stuff2\n"
      "--\n"
      "sufix\n"));
  }

  TEST_F(TestParser, shouldRemoveUsedOptions)
  {
    setArguments(makeArgs("--help", "-x", "-nLOL", "unknown"));

    auto sut = makeSut();

    sut.parse(&argc, argv);

    EXPECT_THAT(argc, Eq(3));
    EXPECT_THAT(argv[1], Eq("-x"s));
    EXPECT_THAT(argv[2], Eq("unknown"s));
  }

  TEST_F(TestParser, shouldThrowWhenUnknownOptionIsMixedWithKnown)
  {
    setArguments(makeArgs("-hxnLOL"));

    auto sut = makeSut();

    ASSERT_THROW(sut.parse(&argc, argv), UnknownOptionError);
  }

  TEST_F(TestParser, shouldReturnNamedArgs)
  {
    setArguments(makeArgs("--name=Leona", "--help", "--number=42", "--custom=c"));

    auto sut = makeSut();

    sut.parse(&argc, argv);

    auto namedArgs = sut.namedArgs();

    EXPECT_THAT(namedArgs["-n"], Eq("Leona"));
    EXPECT_THAT(namedArgs["--name"], Eq("Leona"));
    EXPECT_THAT(namedArgs["--help"], Eq("true"));
    EXPECT_THAT(namedArgs["--number"], Eq("42"));
    EXPECT_THAT(namedArgs["--custom"], Eq("c"));
  }

  TEST_F(TestParser, shouldHandleNoInBooleanOption)
  {
    setArguments(makeArgs("--no-stuff1"));

    auto sut = makeSut();

    EXPECT_THAT(sut.get<Tag::Stuff1>().value(), Eq(false));
  }

  TEST_F(TestParser, shouldHandleBooleanOptionWhenDefaultIsFalse)
  {
    setArguments(makeArgs("--stuff2"));

    auto sut = makeSut();

    EXPECT_THAT(sut.get<Tag::Stuff2>().value(), Eq(true));
  }
}
