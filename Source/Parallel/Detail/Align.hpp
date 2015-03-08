#pragma once

#include <Generated/CacheLineSize.hpp>
#include <cstddef>

namespace Common
{
namespace Parallel
{
namespace Detail
{

template <class T, size_t count = 1>
struct IsCacheFriendly;

template <class T>
struct IsCacheFriendly<T, CACHE_LINE_SIZE>
{
    static constexpr size_t size = sizeof(T) * CACHE_LINE_SIZE;
    static constexpr bool value = (size / CACHE_LINE_SIZE == 1 and size % CACHE_LINE_SIZE == 0);
};

template <class T, size_t count>
struct IsCacheFriendly
{
    static constexpr size_t size = sizeof(T) * count;
    static constexpr bool value = (size / CACHE_LINE_SIZE == 1 and size % CACHE_LINE_SIZE == 0) || IsCacheFriendly<T, count+1>::value;
};

template <class T>
class Align
{
public:
    static size_t unaligned(T* memory)
    {
        return IsCacheFriendly<T>::value and reinterpret_cast<size_t>(memory) % CACHE_LINE_SIZE != 0
            ? (CACHE_LINE_SIZE - (reinterpret_cast<size_t>(memory) % CACHE_LINE_SIZE)) / sizeof(T)
            : 0;
    }

    static constexpr size_t progress = (IsCacheFriendly<T>::value ? CACHE_LINE_SIZE / sizeof(T) : 1);
};

}
}
}
