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

#pragma once

#include "Detail/MatchName.hpp"
#include <string>
#include <sstream>
#include <type_traits>
#include <vector>
#include <regex>

namespace Common::OptionParser
{
  namespace Detail
  {
    std::string join(const std::vector<std::string>& text, std::string sep);

    template <class... Args>
    inline std::vector<std::string> mkVector(std::string name, Args&&... names)
    {
      auto result = std::vector<std::string>();
      result.emplace_back(std::move(name));
      ( result.emplace_back(std::move(names)), ... );
      return result;
    }

    template <class TO>
    inline TO convert(std::string from)
    {
      return TO(from);
    }

    template <>
    inline std::string convert<std::string>(std::string from)
    {
      return from;
    }

    template <>
    inline int convert<int>(std::string from)
    {
      return std::stoi(from);
    }

    template <>
    inline double convert<double>(std::string from)
    {
      return std::stod(from);
    }

    template <>
    inline bool convert<bool>(std::string from)
    {
      return from == "true";
    }
  }

  template <class T>
  struct Option
  {
    template <class... Args>
    explicit Option(std::string name, Args&&... names)
      : _names(Detail::mkVector(std::move(name), std::forward<Args>(names)...))
    {
      if constexpr(isBool)
      {
        _present = true;
        _hasDefault = true;
      }
    }

    std::vector<std::string> names() const
    {
      return _names;
    }

    Option<T>& description(std::string val)
    {
      _description = val;
      return *this;
    }

    std::string description() const
    {
      return _description;
    }

    Option<T>& value(std::string val)
    {
      _value = Detail::convert<T>(val);
      _present = true;

      if (_callback)
        _callback(_value);

      return *this;
    }

    T value() const
    {
      if (_present)
        return _value;
      return _default;
    }

    Option<T>& defaultValue(std::string val)
    {
      _default = Detail::convert<T>(val);
      _hasDefault = true;
      return *this;
    }

    T defaultValue() const
    {
      return _default;
    }

    Option<T>& on(std::function<void (const T&)> callback)
    {
      _callback = callback;
      return *this;
    }

    bool isName(std::string name) const
    {
      return Detail::isName(_names, name);
    }

    bool isNegationName(std::string name) const
    {
      return false;
    }

    template <class F>
    void help(F&& fun) const
    {
      auto _def = std::string();

      if (_hasDefault and _showDefault<T>())
      {
        std::stringstream ss;

        ss << "default: ";
        if constexpr(isBool)
          ss << std::boolalpha;
        ss << _default;

        using namespace std::literals;
        _def = ss.str();
      }

      if (_description.empty() and _def.size() > 0)
        fun(Detail::join(_names, ", "), _def);
      else
      {
        fun(Detail::join(_names, ", "), _description);
        if (_def.size() > 0)
        {
          using namespace std::literals;
          fun(std::string(), "  "s + _def);
        }
      }
    }

    explicit operator bool() const
    {
      return _present or _hasDefault;
    }

    enum { isBool = std::is_same<T, bool>::value };

  private:
    template <class U, class std::enable_if<  std::is_same<U, bool>::value, int>::type = 0>
    bool _showDefault() const
    {
      return _default == true;
    }

    template <class U, class std::enable_if<! std::is_same<U, bool>::value, int>::type = 0>
    bool _showDefault() const
    {
      return true;
    }

    std::function<void (const T&)> _callback;

    std::vector<std::string> _names;
    std::string _description;

    bool _hasDefault = false;
    T _default = T();

    bool _present = false;
    T _value = T();
  };
}
