#pragma once
#include <boost/optional.hpp>

namespace Library
{
namespace Traits
{

template <typename T>
struct IsOptional
{
    static const bool value = false;
};

template <typename T>
struct IsOptional <boost::optional<T>>
{
    typedef T type;
    static const bool value = true;
};

}
}
