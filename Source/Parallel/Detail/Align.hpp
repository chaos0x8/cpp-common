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
