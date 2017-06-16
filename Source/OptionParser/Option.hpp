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

        ss << "  default: ";
        ss << _default;

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
      : _names(Detail::mkVector(std::move(name), std::forward<Args>(names)...))
    {
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
      _value = Detail::convert<bool>(val);

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
