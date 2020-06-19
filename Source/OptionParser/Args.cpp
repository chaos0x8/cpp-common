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

#include "Args.hpp"
#include "Exceptions.hpp"

namespace Common::OptionParser
{
  Args::Args(int* argc, char** argv)
    : _argc(argc), _argv(argv)
  {
  }

  using value_type = const char*;

  const char* Args::zero() const
  {
    return _argv[0];
  }

  const char* Args::operator[](size_t index) const
  {
    return _argv[1u+index];
  }

  size_t Args::size() const
  {
    return static_cast<size_t>(*_argc-1);
  }

  std::string Args::take()
  {
    if (*_argc > 1)
    {
      std::string result = _argv[1];

      for (int i = 2; i < *_argc; ++i)
        std::swap(_argv[i], _argv[i-1]);
      *_argc -= 1;

      return result;
    }
    else
    {
      throw InsufficientOptionsError();
    }
  }
}