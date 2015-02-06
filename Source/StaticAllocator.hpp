#pragma once

#include <memory>
#include <atomic>
#include <vector>
#include <mutex>

namespace Common
{
namespace Detail
{

class StaticAllocatorBase
{
public:
    static void setUp(size_t staticPoolSize, size_t chunkSize);
    static void tearDown();

    static void* _allocate(size_t bytes);
    static inline void* _allocate(size_t bytes, void* hint)
    {
        return _allocate(bytes);
    }
    static void _deallocate(void* p, size_t bytes);

    inline static size_t calcChunkAmount(size_t bytes)
    {
        return (bytes / _chunkSize)
            + (bytes % _chunkSize > 0u ? 1u : 0u);
    }

protected:
    static size_t _staticPoolSize;
    static size_t _chunkSize;
    static size_t _chunkIndexHint;
    static size_t _totalChunkAmount;

    static void* _memory;
    static std::vector<bool> _memoryMap;
    static std::mutex _mutex;
};

}

template <typename T>
class StaticAllocator;

template <>
class StaticAllocator<void> : public Detail::StaticAllocatorBase
{
public:
    using const_pointer = void*;
};

template <typename T>
class StaticAllocator : public Detail::StaticAllocatorBase
{
public:
    StaticAllocator() = default;
    StaticAllocator(const StaticAllocator<T>& other) = default;
    template <typename U>
    StaticAllocator(const StaticAllocator<U>& other) { }

    StaticAllocator<T>& operator = (const StaticAllocator<T>&) = delete;
    StaticAllocator<T>& operator = (StaticAllocator<T>&&) = delete;

    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    template <typename U>
    struct rebind
    {
        using other = StaticAllocator<U>;
    };

    inline pointer allocate(size_type n)
    {
        return static_cast<pointer>(this->_allocate(n * sizeof(T)));
    }

    inline pointer allocate(size_type n, StaticAllocator<void>::const_pointer hint)
    {
        return static_cast<pointer>(this->_allocate(n * sizeof(T), hint));
    }

    inline void deallocate(pointer p, size_type n)
    {
        this->_deallocate(p, n * sizeof(T));
    }

    template <typename U, typename... Args>
    inline void construct(U* p, Args&&... args)
    {
        ::new((void*) p) U(std::forward<Args>(args)...);
    }

    template <typename U>
    inline void destroy(U* p)
    {
        p->~U();
    }
};

template <typename T1, typename T2>
inline bool operator == (const StaticAllocator<T1>& left, const StaticAllocator<T2>& right)
{
    return true;
}

template <typename T1, typename T2>
inline bool operator != (const StaticAllocator<T1>& left, const StaticAllocator<T2>& right)
{
    return false;
}

class StaticAllocatorInitializer
{
public:
    StaticAllocatorInitializer(size_t staticPoolSize, size_t chunkSize)
    {
        Detail::StaticAllocatorBase::setUp(staticPoolSize, chunkSize);
    }
    StaticAllocatorInitializer(const StaticAllocatorInitializer&) = delete;
    StaticAllocatorInitializer(StaticAllocatorInitializer&&) = delete;

    ~StaticAllocatorInitializer()
    {
        Detail::StaticAllocatorBase::tearDown();
    }
};

}
