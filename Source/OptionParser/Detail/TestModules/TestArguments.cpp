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

#include "../Arguments.hpp"
#include "../../TestModules/ArgumentsBuilder.hpp"
#include <gmock/gmock.h>

namespace Common::OptionParser::Detail
{
  using namespace testing;

  struct TestArguments : public Test
  {
    template <class... Args>
    void makeSut(Args&&... a)
    {
      args = makeArgs(std::forward<Args>(a)...);
      std::tie(argc, argv) = args->view();
      sut = std::make_unique<Arguments>(&argc, argv);
    }

    std::unique_ptr<Arguments> sut;
    std::shared_ptr<ArgumentsBuilder> args;

    std::string name, value;

    int argc;
    char** argv;
  };

  TEST_F(TestArguments, shouldIterateThroughOneLetterArguments)
  {
    makeSut("-vpn");

    EXPECT_THAT(sut->takeName(&name), Eq(true));
    EXPECT_THAT(name, Eq("-v"));

    EXPECT_THAT(sut->takeName(&name), Eq(true));
    EXPECT_THAT(name, Eq("-p"));

    EXPECT_THAT(sut->takeName(&name), Eq(true));
    EXPECT_THAT(name, Eq("-n"));

    EXPECT_THAT(sut->takeName(&name), Eq(false));
  }

  TEST_F(TestArguments, shouldIterateThroughOneLetterArgumentsWithValue)
  {
    makeSut("-vn42");

    EXPECT_THAT(sut->takeName(&name), Eq(true));
    EXPECT_THAT(name, Eq("-v"));

    EXPECT_THAT(sut->takeName(&name), Eq(true));
    EXPECT_THAT(name, Eq("-n"));

    EXPECT_THAT(sut->takeValue(&value), Eq(true));
    EXPECT_THAT(value, Eq("42"));

    EXPECT_THAT(sut->takeName(&name), Eq(false));
    EXPECT_THAT(sut->takeValue(&value), Eq(false));
  }

  TEST_F(TestArguments, shouldIterateThroughWordArguments)
  {
    makeSut("--version", "--name");

    EXPECT_THAT(sut->takeName(&name), Eq(true));
    EXPECT_THAT(name, Eq("--version"));
    EXPECT_THAT(sut->takeName(&name), Eq(false));

    EXPECT_THAT(sut->next(), Eq(true));

    EXPECT_THAT(sut->takeName(&name), Eq(true));
    EXPECT_THAT(name, Eq("--name"));
    EXPECT_THAT(sut->takeName(&name), Eq(false));

    EXPECT_THAT(sut->next(), Eq(false));

    EXPECT_THAT(sut->takeName(&name), Eq(false));
  }

  TEST_F(TestArguments, shouldIterateThroughWordArgumentsWithValue)
  {
    makeSut("--version=10", "--name", "Leona");

    EXPECT_THAT(sut->takeName(&name), Eq(true));
    EXPECT_THAT(name, Eq("--version"));
    EXPECT_THAT(sut->takeValue(&value), Eq(true));
    EXPECT_THAT(value, Eq("10"));

    EXPECT_THAT(sut->next(), Eq(true));

    EXPECT_THAT(sut->takeName(&name), Eq(true));
    EXPECT_THAT(name, Eq("--name"));

    EXPECT_THAT(sut->next(), Eq(true));

    EXPECT_THAT(sut->takeValue(&value), Eq(true));
    EXPECT_THAT(value, Eq("Leona"));
  }

  TEST_F(TestArguments, shouldReturnTrueIfWordArgumentShouldContainValue)
  {
    makeSut("--name=");

    EXPECT_THAT(sut->takeName(&name), Eq(true));
    EXPECT_THAT(name, Eq("--name"));

    EXPECT_THAT(sut->containsValue(), Eq(true));
    EXPECT_THAT(sut->takeValue(&value), Eq(false));
  }

  TEST_F(TestArguments, shouldReturnTrueIfWordArgumentContainValue)
  {
    makeSut("--help=true");

    EXPECT_THAT(sut->takeName(&name), Eq(true));
    EXPECT_THAT(name, Eq("--help"));

    EXPECT_THAT(sut->containsValue(), Eq(true));
  }
}
