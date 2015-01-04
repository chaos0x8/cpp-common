#pragma once
#include <utility>

namespace Common
{
namespace Traits
{

template <typename T>
struct Reference
{
    static T& get(T& object)
    {
        return object;
    }
};

template <typename T>
struct Reference<T*>
{
    static T& get(T* object)
    {
        return *object;
    }
};

template <typename T>
struct Reference<std::reference_wrapper<T>>
{
    static T& get(std::reference_wrapper<T> object)
    {
        return object;
    }
};

template <typename T>
auto& reference(T& object)
{
    return Reference<T>::get(object);
}

}
}
