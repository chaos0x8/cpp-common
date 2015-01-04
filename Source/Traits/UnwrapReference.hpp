#pragma once
#include <utility>

namespace Common
{
namespace Traits
{

template <typename T>
struct UnwrapReference
{
    typedef T type;
};

template <typename T>
struct UnwrapReference<std::reference_wrapper<T>>
{
    typedef T type;
};

}
}
