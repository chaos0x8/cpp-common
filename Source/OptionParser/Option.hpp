#pragma once

#include <string>
#include <sstream>
#include <type_traits>

namespace Common::OptionParser
{
  namespace Detail
  {
    bool isTrueAlike(std::string val);
  }

  template <class T>
  struct Option
  {
    explicit Option(std::string name)
      : _name(name),
        _value()
    {
      if (std::is_same<T, bool>::value)
        defaultValue("false");
    }

    std::string name() const
    {
      return _name;
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

    template <class U, class std::enable_if<
      std::is_same<U, std::string>::value and
      std::is_same<T, std::string>::value, int>::type = 0
    >
    Option<T>& value(U val)
    {
      _assign( val );
      return *this;
    }

    template <class U, class std::enable_if<
      std::is_same<U, std::string>::value and
      ! std::is_same<T, std::string>::value and
      ! std::is_same<T, int>::value and
      ! std::is_same<T, bool>::value, int>::type = 0
    >
    Option<T>& value(U val)
    {
      _assign( T(val) );
      return *this;
    }

    Option<T>& value(const char* val)
    {
      return value(std::string(val));
    }

    template <class U, class std::enable_if<
      std::is_same<U, std::string>::value and
      std::is_same<T, int>::value, int>::type = 0
    >
    Option<T>& value(U val)
    {
      _assign( std::stoi(val) );
      return *this;
    }

    template <class U, class std::enable_if<
      std::is_same<U, std::string>::value and
      std::is_same<T, bool>::value, int>::type = 0
    >
    Option<T>& value(U val)
    {
      _assign( Detail::isTrueAlike(val) );
      return *this;
    }

    T value() const
    {
      return _value;
    }

    template <typename U>
    Option<T>& defaultValue(U val)
    {
      _default = true;
      return value(val);
    }

    Option<T>& set()
    {
      if (std::is_same<T, bool>::value)
        value("true");
      return *this;
    }

    std::string info() const
    {
      std::stringstream ss;
      ss << "\t" << _name;

      if (_description.size() > 0)
        ss << "\t" << _description;

      if (_default)
      {
        ss << "\n\t\tdefault: ";
        if (std::is_same<T, bool>::value)
          ss << std::boolalpha;
        ss << _value;
      }

      return ss.str();
    }

    explicit operator bool() const
    {
      return _present;
    }

  private:
    void _assign(T val)
    {
      _value = std::move(val);
      _present = true;
    }

    std::string _name;
    std::string _description;

    bool _default = false;
    bool _present = false;
    T _value;
  };
}
