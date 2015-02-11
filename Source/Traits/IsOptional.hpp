#pragma once
#include <boost/optional.hpp>

namespace Library
{
namespace Traits
{

template <typename T>
struct IsOptional
{
    static constexpr bool value = false;
};

template <typename T>
struct IsOptional <boost::optional<T>>
{
    typedef T type;
    static constexpr bool value = true;
};

}
}
