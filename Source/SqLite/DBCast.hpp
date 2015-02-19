#pragma once
#include <Traits/IsOptional.hpp>
#include <string>
#include <sstream>
#include <type_traits>
#include <boost/optional.hpp>

namespace Library
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
