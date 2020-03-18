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

#include "ArgumentsBuilder.hpp"
#include <cstring>

namespace Common::OptionParser
{
  ArgumentsBuilder::ArgumentsBuilder()
  {
    push_back("appName");
  }
  ArgumentsBuilder::~ArgumentsBuilder()
  {
    reset();
  }

  ArgumentsBuilder::ArgumentsBuilder(ArgumentsBuilder&& other) : argc(other.argc), argv(other.argv)
  {
    other.argv = nullptr;
  }

  ArgumentsBuilder& ArgumentsBuilder::operator=(ArgumentsBuilder&& other)
  {
    reset();

    argc = other.argc;
    argv = other.argv;

    other.argv = nullptr;

    return *this;
  }

  void ArgumentsBuilder::push_back(std::string arg)
  {
    auto new_argc = argc + 1;
    auto new_argv = new char*[new_argc];

    for (int i = 0; i < argc; ++i)
    {
      const auto size = std::strlen(argv[i]) + 1;
      new_argv[i] = new char[size];
      std::memcpy(new_argv[i], argv[i], size);
    }

    new_argv[argc] = new char[arg.size() + 1];
    std::memcpy(new_argv[argc], arg.data(), arg.size() + 1);

    reset();

    argc = new_argc;
    argv = new_argv;
  }

  void ArgumentsBuilder::reset()
  {
    if (argv)
    {
      for (int i = 0; i < argc; ++i)
        delete[] argv[i];
      delete[] argv;
    }

    argc = 0;
    argv = nullptr;
  }

  std::tuple<int, char**> ArgumentsBuilder::view() const
  {
    return std::make_tuple(argc, argv);
  }
}

