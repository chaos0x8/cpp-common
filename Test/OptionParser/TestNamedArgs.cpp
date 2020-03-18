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

#include "OptionParser/NamedArgs.hpp"
#include "OptionParser/Exceptions.hpp"
#include <gmock/gmock.h>

namespace Common::OptionParser
{
  using namespace testing;
  using namespace std::literals;

  struct TestNamedArgs : public Test
  {
    TestNamedArgs()
      : sut({ NamedArgs::Item({"--n1A"s, "--n1B"s}, "val1"s),
              NamedArgs::Item({"--n2"s}, "val2"s)})
    {
    }

    NamedArgs sut;
  };

  TEST_F(TestNamedArgs, shouldReturnValueByAnyName)
  {
    EXPECT_THAT(sut["--n1A"], Eq("val1"s));
    EXPECT_THAT(sut["--n1B"], Eq("val1"s));
    EXPECT_THAT(sut["--n2"], Eq("val2"s));
  }

  TEST_F(TestNamedArgs, shouldThrowUnknownOptionErrorWhenNameIsNotKnown)
  {
    EXPECT_THROW(sut["--unknown"], UnknownOptionError);
  }
}
