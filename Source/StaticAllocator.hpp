#pragma once

#include <memory>
#include <atomic>

namespace Common
{

class StaticMemory
{
public:
    explicit StaticMemory(size_t size);
    StaticMemory(const StaticMemory&) = delete;
    StaticMemory(StaticMemory&&) = delete;
    ~StaticMemory();

    StaticMemory& operator = (const StaticMemory&) = delete;
    StaticMemory& operator = (StaticMemory&&) = delete;

    void init();

    void* allocate(size_t s);
    void deallocate(void* p, size_t s);

private:
    const size_t size;

    void* memory = nullptr;
    std::atomic<size_t> location;
};

template <typename T>
class StaticAllocator
{
public:
    explicit StaticAllocator(std::shared_ptr<StaticMemory> memory)
        : memory(memory)
    {
    }
    StaticAllocator(const StaticAllocator<T>& other)
        : memory(other.memory)
    {
    }
    template <typename U>
    StaticAllocator(const StaticAllocator<U>& other)
        : memory(other.memory)
    {
    }

    StaticAllocator<T>& operator = (const StaticAllocator<T>&) = delete;
    StaticAllocator<T>& operator = (StaticAllocator<T>&&) = delete;

    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    inline pointer allocate(size_type n, std::allocator<void>::const_pointer hint = 0)
    {
        return static_cast<pointer>(memory->allocate(n * sizeof(T)));
    }

    inline void deallocate(pointer p, size_type n)
    {
        memory->deallocate(p, n * sizeof(T));
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

    std::shared_ptr<StaticMemory> memory;
};

}
