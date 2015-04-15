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

#include <utility>

namespace Common
{
namespace GL
{
namespace Detail
{

template <typename T, typename Deleter, typename Allocator>
class SmartResource
{
public:
    SmartResource() noexcept = default;
    SmartResource(const SmartResource<T, Deleter, Allocator>&) = delete;
    SmartResource(SmartResource<T, Deleter, Allocator>&& other) noexcept
        : resourceId(std::move(other.resourceId))
    {
        other.resourceId = T();
    }
    ~SmartResource() noexcept
    {
        clear();
    }

    template <typename... Args>
    static SmartResource<T, Deleter, Allocator> allocate(Args&&... args)
    {
        Allocator allocator = Allocator();
        return SmartResource<T, Deleter, Allocator>(allocator(args...));
    }

    operator T () const noexcept
    {
        return resourceId;
    }

    SmartResource<T, Deleter, Allocator>& operator = (const SmartResource<T, Deleter, Allocator>&) = delete;
    SmartResource<T, Deleter, Allocator>& operator = (SmartResource<T, Deleter, Allocator>&& other) noexcept
    {
        this->clear();
        this->resourceId = std::move(other.resourceId);
        other.resourceId = T();
        return *this;
    }

private:
    explicit SmartResource(const T& resourceId) noexcept
        : resourceId(resourceId)
    {
    }

    void clear() noexcept
    {
        Deleter deleter = Deleter();
        if (resourceId)
            deleter(resourceId);
        resourceId = T();
    }

    T resourceId = T();
};

}
}
}
