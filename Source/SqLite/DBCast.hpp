/*!
 *  \author <https://github.com/chaos0x8>
 *  \copyright
 *  Copyright (c) 2015 - 2016, <https://github.com/chaos0x8>
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

#include <boost/algorithm/string/replace.hpp>
#include <boost/lexical_cast.hpp>
#include <optional>
#include <sstream>
#include <string>
#include <type_traits>

namespace Common {
  namespace SqLite {
    namespace DBCast {
      namespace Detail {

        template <class T> struct ConvertableNumber {
          static constexpr bool value =
            std::is_same<unsigned, T>::value ||
            std::is_same<unsigned long, T>::value ||
            std::is_same<unsigned long long, T>::value ||
            std::is_same<int, T>::value || std::is_same<long, T>::value ||
            std::is_same<long long, T>::value ||
            std::is_same<float, T>::value || std::is_same<double, T>::value ||
            std::is_same<long double, T>::value;
        };

        template <typename T> inline T convNumber(const std::string&);

        template <>
        inline unsigned convNumber<unsigned>(const std::string& data) {
          return std::stoul(data);
        }

        template <>
        inline unsigned long convNumber<unsigned long>(
          const std::string& data) {
          return std::stoul(data);
        }

        template <>
        inline unsigned long long convNumber<unsigned long long>(
          const std::string& data) {
          return std::stoull(data);
        }

        template <> inline int convNumber<int>(const std::string& data) {
          return std::stoi(data);
        }

        template <> inline long convNumber<long>(const std::string& data) {
          return std::stol(data);
        }

        template <>
        inline long long convNumber<long long>(const std::string& data) {
          return std::stoll(data);
        }

        template <> inline float convNumber<float>(const std::string& data) {
          return std::stof(data);
        }

        template <> inline double convNumber<double>(const std::string& data) {
          return std::stod(data);
        }

        template <>
        inline long double convNumber<long double>(const std::string& data) {
          return std::stold(data);
        }
      } // namespace Detail

      template <typename T> inline T fromDBFormat(const std::string& data) {
        if constexpr (std::is_same_v<T, std::string>) {
          return data;
        } else if constexpr (Detail::ConvertableNumber<T>::value) {
          return Detail::convNumber<T>(data);
        } else {
          return boost::lexical_cast<T>(data);
        }
      }

      template <typename T> inline std::string toDBFormat(const T& val) {
        return boost::lexical_cast<std::string>(val);
      }

      inline std::string toDBFormat(const double& val) {
        std::ostringstream ss;
        ss << val;
        return ss.str();
      }

      inline std::string toDBFormat(const std::string& value) {
        return std::string("'") + boost::replace_all_copy(value, "'", "''") +
               "'";
      }

      inline std::string toDBFormat(const char* value) {
        return toDBFormat(std::string(value));
      }

      template <typename T>
      inline std::string toDBFormat(const std::optional<T>& val) {
        if (!val)
          return "null";
        return toDBFormat(*val);
      }

    } // namespace DBCast
  }   // namespace SqLite
} // namespace Common
