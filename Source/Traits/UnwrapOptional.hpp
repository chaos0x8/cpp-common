#pragma once
#include <boost/optional.hpp>

namespace Common
{
namespace Traits
{

template <typename T>
struct UnwrapOptional
{
    typedef T type;
};

template <typename T>
struct UnwrapOptional<boost::optional<T>>
{
    typedef T type;
};

}
}
