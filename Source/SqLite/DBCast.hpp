/*!
    \author <https://github.com/chaos0x8>
    \copyright
    Copyright (c) 2015, <https://github.com/chaos0x8>

    \copyright
    Permission to use, copy, modify, and/or distribute this software for any
    purpose with or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    \copyright
    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#pragma once
#include <Traits/IsOptional.hpp>
#include <string>
#include <sstream>
#include <type_traits>
#include <boost/optional.hpp>

namespace Common
{
namespace SqLite
{
namespace DBCast
{

template <typename T>
inline T fromDBFormat(
    const std::string& data,
    typename std::enable_if<!Traits::IsOptional<T>::value &&  std::is_same<T, std::string>::value>::type* = 0)
{
    return data;
}

template <typename T>
inline T fromDBFormat(
    const std::string& data,
    typename std::enable_if<!Traits::IsOptional<T>::value && !std::is_same<T, std::string>::value>::type* = 0)
{
    std::stringstream ss;
    ss << data;

    T result = T();
    ss >> result;

    return result;
}

template <typename T>
inline T fromDBFormat(
    const std::string& data,
    typename std::enable_if< Traits::IsOptional<T>::value>::type* = 0)
{
    if (data.empty())
        return T();
    return fromDBFormat<typename Traits::IsOptional<T>::type>(data);
}

std::string toDBFormat(const std::string& value);
std::string toDBFormat(const char* value);

template <typename T>
inline std::string toDBFormat(const T& val)
{
    std::stringstream ss;
    ss << val;
    return ss.str();
}

template <typename T>
inline std::string toDBFormat(const boost::optional<T>& val)
{
    if (!val)
        return "null";
    return toDBFormat(*val);
}

}
}
}
