#pragma once

#include <memory>
#include <atomic>
#include <mutex>
#include <thread>
#include <cassert>

namespace Common
{
namespace Detail
{

class MemoryPool
{
public:
    MemoryPool(size_t poolSize, size_t chunkSize) __attribute__((cold));
    MemoryPool(const MemoryPool&) = delete;
    MemoryPool(MemoryPool&&) = delete;
    ~MemoryPool() __attribute__((cold));

    MemoryPool& operator = (const MemoryPool&) = delete;
    MemoryPool& operator = (MemoryPool&&) = delete;

    size_t calcChunkAmount(size_t bytes) const __attribute__((hot, always_inline));

    void* _allocate(size_t bytes) __attribute__((hot));
    bool _deallocate(void* p, size_t bytes) __attribute__((hot));

private:
    const size_t _poolSize;
    const size_t _chunkSize;
    const size_t _chunkAmount;

    size_t _chunkHint = 0;
    std::mutex _mutex;

    void* _heap = nullptr;
    bool* _memoryMap = nullptr;
    void* _memory = nullptr;
};

class ParallelMemoryPool
{
public:
    static void setUp(size_t poolSize, size_t chunkSize, size_t poolAmount) __attribute__((cold));
    static void tearDown() __attribute__((cold));

    static void* _allocate(size_t bytes) __attribute__((hot));
    static void* _allocate(size_t bytes, void* hint) __attribute__((hot, always_inline));

    static void _deallocate(void* p, size_t bytes) __attribute__((hot));

private:
    static size_t _poolAmount;
    static MemoryPool* _memoryPools;
};

inline void* ParallelMemoryPool::_allocate(size_t bytes, void* hint)
{
    return _allocate(bytes);
}

}

template <typename T>
class StaticAllocator;

template <>
class StaticAllocator<void> : public Detail::ParallelMemoryPool
{
public:
    using const_pointer = void*;
};

template <typename T>
class StaticAllocator : public Detail::ParallelMemoryPool
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

    pointer allocate(size_type n) __attribute__((hot, always_inline));
    pointer allocate(size_type n, StaticAllocator<void>::const_pointer hint) __attribute__((hot, always_inline));
    void deallocate(pointer p, size_type n) __attribute__((hot, always_inline));

    template <typename U, typename... Args>
    inline void construct(U* p, Args&&... args) __attribute__((hot, always_inline));

    template <typename U>
    inline void destroy(U* p) __attribute__((hot, always_inline));
};

template <typename T>
inline typename StaticAllocator<T>::pointer StaticAllocator<T>::allocate(size_type n)
{
    return static_cast<pointer>(this->_allocate(n * sizeof(T)));
}

template <typename T>
inline typename StaticAllocator<T>::pointer StaticAllocator<T>::allocate(size_type n, StaticAllocator<void>::const_pointer hint)
{
    return static_cast<pointer>(this->_allocate(n * sizeof(T), hint));
}

template <typename T>
inline void StaticAllocator<T>::deallocate(pointer p, size_type n)
{
    this->_deallocate(p, n * sizeof(T));
}

template <typename T>
template <typename U, typename... Args>
inline void StaticAllocator<T>::construct(U* p, Args&&... args)
{
    ::new((void*) p) U(std::forward<Args>(args)...);
}

template <typename T>
template <typename U>
inline void StaticAllocator<T>::destroy(U* p)
{
    p->~U();
}

template <typename T1, typename T2>
inline bool operator == (const StaticAllocator<T1>& left, const StaticAllocator<T2>& right) __attribute__((always_inline));
template <typename T1, typename T2>
inline bool operator == (const StaticAllocator<T1>& left, const StaticAllocator<T2>& right)
{
    return true;
}

template <typename T1, typename T2>
inline bool operator != (const StaticAllocator<T1>& left, const StaticAllocator<T2>& right) __attribute__((always_inline));
template <typename T1, typename T2>
inline bool operator != (const StaticAllocator<T1>& left, const StaticAllocator<T2>& right)
{
    return false;
}

class StaticAllocatorInitializer
{
public:
    StaticAllocatorInitializer(size_t staticPoolSize, size_t chunkSize, size_t poolAmount = std::thread::hardware_concurrency())
    {
        assert(poolAmount);

        Detail::ParallelMemoryPool::setUp(staticPoolSize / poolAmount, chunkSize, poolAmount);
    }
    StaticAllocatorInitializer(const StaticAllocatorInitializer&) = delete;
    StaticAllocatorInitializer(StaticAllocatorInitializer&&) = delete;

    ~StaticAllocatorInitializer()
    {
        Detail::ParallelMemoryPool::tearDown();
    }
};

}
