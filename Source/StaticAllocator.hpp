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
    MemoryPool(size_t poolSize, size_t chunkSize);
    MemoryPool(const MemoryPool&) = delete;
    MemoryPool(MemoryPool&&) = delete;
    ~MemoryPool();

    MemoryPool& operator = (const MemoryPool&) = delete;
    MemoryPool& operator = (MemoryPool&&) = delete;

    size_t calcChunkAmount(size_t bytes) const;

    void* _allocate(size_t bytes);
    bool _deallocate(void* p, size_t bytes);

private:
    const size_t _poolSize;
    const size_t _chunkSize;
    const size_t _chunkAmount;

    void* _heap = nullptr;
    std::mutex* _mutex = nullptr;
    size_t* _chunkHint = nullptr;
    bool* _memoryMap = nullptr;
    void* _memory = nullptr;
};

class ParallelMemoryPool
{
public:
    static void setUp(size_t poolSize, size_t chunkSize, size_t poolAmount);
    static void tearDown();

    static void* _allocate(size_t bytes);
    static inline void* _allocate(size_t bytes, void* hint)
    {
        return _allocate(bytes);
    }

    static void _deallocate(void* p, size_t bytes);

private:
    static size_t* _poolAmount;
    static MemoryPool* _memoryPools;
};

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
