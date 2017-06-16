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

#include <utility>
#include <tuple>
#include <sstream>
#include <vector>
#include "Detail/Arguments.hpp"
#include "Exceptions.hpp"

namespace Common::OptionParser
{
  namespace Detail
  {
    template <class TAG, TAG VALUE, int N, class OPT, class... Args>
    struct Find
    {
      enum
      {
        value = (OPT::value() == VALUE ? N : Find<TAG, VALUE, N + 1, Args...>::value)
      };
    };

    template <class TAG, TAG VALUE, int N, class OPT>
    struct Find<TAG, VALUE, N, OPT>
    {
      enum
      {
        value = (OPT::value() == VALUE ? N : N + 1)
      };
    };
  }

  template <class TAG, TAG VALUE, class OPTION>
  struct Tagged
  {
    template <class... Args>
    Tagged(Args&&... args) : option(std::forward<Args>(args)...)
    {
    }

    static constexpr TAG value()
    {
      return VALUE;
    }

    OPTION option;
  };

  template <class TAG, class A0, class... Args>
  struct Parser
  {
    Parser(A0&& a0, Args&&... args) : options(std::forward<A0>(a0), std::forward<Args>(args)...)
    {
    }

    template <TAG VALUE>
    auto& get()
    {
      return std::get<Detail::Find<TAG, VALUE, 0, A0, Args...>::value>(options).option;
    }

    void parse(int* argc, char** argv)
    {
      auto args = Detail::Arguments(argc, argv);

      std::string name, value;
      while (args.takeName(&name) or (args.next() and args.takeName(&name)))
      {
        each<0, 1+sizeof...(Args)>([&](auto& opt) {
          if (opt.matchName(name))
          {
            if constexpr(opt.isBool)
            {
              if (args.containsValue())
                throw UnexpectedValueError(name);
              opt.set();
            }
            else
            {
              if (args.containsValue())
              {
                if (!args.takeValue(&value))
                  throw MissingValueError(name);
              }
              else if (!args.takeValue(&value) and (!args.next() or !args.takeValue(&value)))
                throw MissingValueError(name);

              opt.value(value);
            }
          }
        });
      }
    }

    void addHelpPrefix(std::string line)
    {
      _helpPrefix.push_back(line);
    }

    void addHelpSufix(std::string line)
    {
      _helpSufix.push_back(line);
    }

    std::string help() const
    {
      auto lines = std::vector<std::tuple<std::string, std::string>>();

      each<0, 1+sizeof...(Args)>([&](const auto& opt) {
        opt.help([&](auto col1, auto col2) {
          lines.emplace_back(col1, col2);
        });
      });

      auto col1Width = 0ul;

      for (const auto& l : lines)
        col1Width = std::max(col1Width, std::get<0>(l).size());

      std::stringstream ss;

      for (const auto& l : _helpPrefix)
        ss << l << "\n";

      for (const auto& l : lines)
      {
        auto width = col1Width + 8 - std::get<0>(l).size();
        auto spaces = std::string(width, ' ');


        ss << "  " << std::get<0>(l) << spaces << std::get<1>(l) << "\n";
      }

      for (const auto& l : _helpSufix)
        ss << l << "\n";

      return ss.str();
    }

  private:
    template <int I, int MAX, typename F, class std::enable_if<I != MAX, int>::type = 0>
    void each(F&& fun)
    {
      fun(std::get<I>(options).option);
      each<I + 1, MAX>(std::forward<F>(fun));
    }

    template <int I, int MAX, typename F, class std::enable_if<I == MAX, int>::type = 0>
    void each(F&& fun)
    {
    }

    template <int I, int MAX, typename F, class std::enable_if<I != MAX, int>::type = 0>
    void each(F&& fun) const
    {
      fun(std::get<I>(options).option);
      each<I + 1, MAX>(std::forward<F>(fun));
    }

    template <int I, int MAX, typename F, class std::enable_if<I == MAX, int>::type = 0>
    void each(F&& fun) const
    {
    }

    std::vector<std::string> _helpPrefix;
    std::vector<std::string> _helpSufix;

    std::tuple<A0, Args...> options;
  };

  template <class TAG, class... Args>
  auto makeParser(Args&&... args)
  {
    return Parser<TAG, Args...>(std::forward<Args>(args)...);
  }
}
