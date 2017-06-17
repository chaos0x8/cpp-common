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

#include "Arguments.hpp"
#include "../Exceptions.hpp"
#include <regex>
#include <iostream>

namespace Common::OptionParser::Detail
{
  using namespace std::literals;

  Arguments::Arguments(int* argc, char** argv) : _argc(argc), _argv(argv)
  {
  }

  bool Arguments::takeName(std::string* name)
  {
    std::cmatch m;

    if (_it < *_argc)
    {
      const auto len = std::strlen(_argv[_it]);
      if (std::regex_match(_argv[_it], m, std::regex("^-\\w.*")))
      {
        if (_kt == 0)
          ++_kt;

        if (_kt < len)
        {
          *name = "-"s + _argv[_it][_kt];
          ++_kt;
          return true;
        }
      }
      else if (_kt < len)
      {
        auto equal = std::find(_argv[_it]+_kt, _argv[_it]+len, '=');
        auto equalPos = std::distance(_argv[_it], equal);

        if (equal != _argv[_it]+len)
        {
          *name = std::string(_argv[_it]).substr(_kt, equalPos);
          _kt = equalPos+1;
          _containsValue = true;
          return true;
        }
        else
        {
          _kt = len;
          *name = _argv[_it];
          return true;
        }
      }
    }

    return false;
  }

  bool Arguments::takeValue(std::string* value)
  {
    std::cmatch m;

    if (_it < *_argc)
    {
      if (_kt < std::strlen(_argv[_it]))
      {
        *value = &_argv[_it][_kt];
        _kt = std::strlen(_argv[_it]);
        ++_matched;
        return true;
      }
    }

    return false;
  }

  bool Arguments::next()
  {
    if (_matched > 0)
    {
      for (int i = _it+1; i < *_argc; ++i)
        std::swap(_argv[i-1], _argv[i]);
      *_argc -= 1;
    }
    else
    {
      ++_it;
    }

    _kt = 0;
    _matched = 0;
    _containsValue = false;

    return _it < *_argc;
  }

  bool Arguments::containsValue() const
  {
    return _containsValue;
  }

  void Arguments::setMatched()
  {
    ++_matched;
  }

  void Arguments::setNotMatched(std::string option)
  {
    if (_matched > 0)
      throw UnknownOptionError(option);
  }
}
