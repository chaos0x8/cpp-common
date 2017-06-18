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

#include "../Args.hpp"
#include "../Exceptions.hpp"
#include "ArgumentsBuilder.hpp"
#include <gmock/gmock.h>

namespace Common::OptionParser
{
  using namespace testing;
  using namespace std::literals;

  struct TestArgs : public Test
  {
    TestArgs()
      : _args(makeArgs("arg1", "arg2", "arg3")),
        _view(_args->view()),
        sut(&std::get<int>(_view), std::get<char**>(_view))
    {
    }


  private:
    std::shared_ptr<ArgumentsBuilder> _args;
    std::tuple<int, char**> _view;

  public:
    Args sut;
  };

  TEST_F(TestArgs, shouldAccessZerothArgument)
  {
    EXPECT_THAT(sut.zero(), Eq("appName"s));
  }

  TEST_F(TestArgs, shouldReturnSizeWithoutZeroArgument)
  {
    EXPECT_THAT(sut.size(), Eq(3u));
  }

  TEST_F(TestArgs, shouldReturnArgument)
  {
    EXPECT_THAT(sut[1], Eq("arg2"s));
  }

  TEST_F(TestArgs, shouldTakeArguments)
  {
    EXPECT_THAT(sut.take(), Eq("arg1"s));
    EXPECT_THAT(sut.take(), Eq("arg2"s));
    EXPECT_THAT(sut.take(), Eq("arg3"s));
  }

  TEST_F(TestArgs, throwWhenNoMoreArgumentsToTake)
  {
    while (sut.size() > 0)
      sut.take();

    ASSERT_THROW(sut.take(), InsufficientOptionsError);
  }
}
