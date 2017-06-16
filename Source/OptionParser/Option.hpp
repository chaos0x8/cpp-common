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
    bool matchName(const std::vector<std::string>& names, std::string expected);
  }

  template <class T>
  struct Option
  {
    template <class... Args>
    explicit Option(std::string name, Args&&... names)
      : _value()
    {
      _names.push_back(std::move(name));
      ( _names.push_back(std::move(names)), ... );
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

    template <class U>
    Option<T>& value(U val)
    {
      _value = _convert(val);
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

    template <typename U>
    Option<T>& defaultValue(U val)
    {
      _default = _convert(val);
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

    Option<T>& set()
    {
      return *this;
    }

    bool matchName(std::string name) const
    {
      return Detail::matchName(_names, name);
    }

    template <class F>
    void help(F&& fun) const
    {
      fun(Detail::join(_names, ", "), _description);

      if (_hasDefault)
      {
        std::stringstream ss;

        if (_hasDefault)
        {
          ss << "  default: ";
          ss << _default;
        }

        using namespace std::literals;
        fun(""s, ss.str());
      }
    }

    explicit operator bool() const
    {
      return _present or _hasDefault;
    }

    enum { isBool = false };

  private:
    template <class U, class std::enable_if<
      std::is_same<U, std::string>::value and
      std::is_same<T, std::string>::value, int>::type = 0
    >
    T _convert(U val)
    {
      return val;
    }

    template <class U, class std::enable_if<
      std::is_same<U, std::string>::value and
      ! std::is_same<T, std::string>::value and
      ! std::is_same<T, int>::value, int>::type = 0
    >
    T _convert(U val)
    {
      return T(val);
    }

    template <class U, class std::enable_if<
      std::is_same<U, std::string>::value and
      std::is_same<T, int>::value, int>::type = 0
    >
    T _convert(U val)
    {
      return std::stoi(val);
    }

    T _convert(const char* val)
    {
      return _convert(std::string(val));
    }

    std::function<void (const T&)> _callback;

    std::vector<std::string> _names;
    std::string _description;

    bool _hasDefault = false;
    T _default = T();

    bool _present = false;
    T _value = T();
  };

  template <>
  struct Option<bool>
  {
    template <class... Args>
    explicit Option(std::string name, Args&&... names)
      : _value()
    {
      _names.push_back(std::move(name));
      ( _names.push_back(std::move(names)), ... );
    }

    Option<bool>& description(std::string val)
    {
      _description = val;
      return *this;
    }

    std::string description() const
    {
      return _description;
    }

    template <class U>
    Option<bool>& value(U val)
    {
      _value = std::string(val) == "true";

      if (_callback)
        _callback(_value);

      return *this;
    }

    bool value() const
    {
      return _value;
    }

    Option<bool>& on(std::function<void (bool)> callback)
    {
      _callback = callback;
      return *this;
    }

    Option<bool>& set()
    {
      _value = true;
      return *this;
    }

    bool matchName(std::string name) const
    {
      return Detail::matchName(_names, name);
    }

    template <class F>
    void help(F&& fun) const
    {
      fun(Detail::join(_names, ", "), _description);
    }

    explicit operator bool() const
    {
      return _value;
    }

    enum { isBool = true };

  private:
    std::function<void (bool)> _callback;

    std::vector<std::string> _names;
    std::string _description;

    bool _value = false;
  };
}
