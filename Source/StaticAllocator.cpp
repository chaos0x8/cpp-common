#include <Generated/CacheLineSize.hpp>
#include <StaticAllocator.hpp>
#include <algorithm>
#include <stdexcept>

#ifndef NDEBUG
#include <iostream>
#endif

namespace Common
{
namespace Detail
{

template <class T = void>
struct Align
{
private:
    static constexpr size_t part_amount = sizeof(T) / CACHE_LINE_SIZE + (sizeof(T) % CACHE_LINE_SIZE == 0ul ? 0ul : 1ul);
    static constexpr size_t value = CACHE_LINE_SIZE * part_amount;

public:
    static inline constexpr size_t memorySize(size_t n)
    {
        return n * value + CACHE_LINE_SIZE;
    }
    static inline constexpr T* item(void* p, size_t index)
    {
        return reinterpret_cast<T*>((reinterpret_cast<size_t>(p) + value * index + CACHE_LINE_SIZE) - (reinterpret_cast<size_t>(p) % CACHE_LINE_SIZE));
    }
};

template <>
struct Align<void>
{
    static inline constexpr void* memory(void* p)
    {
        return reinterpret_cast<void*>((reinterpret_cast<size_t>(p) + CACHE_LINE_SIZE) - (reinterpret_cast<size_t>(p) % CACHE_LINE_SIZE));
    }
};

using AlignPool = Align<MemoryPool>;

MemoryPool::MemoryPool(size_t poolSize, size_t chunkSize)
    : _poolSize(poolSize),
      _chunkSize(chunkSize),
      _chunkAmount(poolSize / chunkSize)
{
    assert(_chunkAmount);
    assert(_poolSize % _chunkSize == 0);

    const size_t extraMemory = sizeof(bool) * _chunkAmount;
    const size_t extraChunks = calcChunkAmount(extraMemory);

    _heap = ::operator new(extraChunks * _chunkSize + _poolSize + CACHE_LINE_SIZE);
    _memoryMap = reinterpret_cast<bool*>(Align<>::memory(_heap));
    _memory = Align<>::memory(_heap) + _chunkSize * extraChunks;

    std::fill(_memoryMap, _memoryMap + _chunkAmount, false);
}

MemoryPool::~MemoryPool()
{
    ::operator delete((void*) _heap);
}

inline size_t MemoryPool::calcChunkAmount(size_t bytes) const
{
    return (bytes / _chunkSize)
        + (bytes % _chunkSize > 0u ? 1u : 0u);
}

#define ALLOCATE_ALGORITHM(startIndex, endIndex) \
    for (size_t i = startIndex; i < endIndex; ++i) \
    { \
        if (_memoryMap[i] == false) \
        { \
            if (++assignedChunks == requiredChunks) \
            { \
                _chunkHint = i + 1u; \
                const size_t firstAssignedChunkIndex = (i + 1u) - requiredChunks; \
                std::fill(_memoryMap + firstAssignedChunkIndex, _memoryMap + firstAssignedChunkIndex + requiredChunks, true); \
                return _memory + (firstAssignedChunkIndex * _chunkSize); \
            } \
        } \
        else \
        { \
            assignedChunks = 0; \
        } \
    }

inline void* MemoryPool::_allocate(size_t bytes)
{
    if (_mutex.try_lock())
    {
        std::unique_lock<std::mutex> lock(_mutex, std::adopt_lock);

        const size_t requiredChunks = calcChunkAmount(bytes);

        size_t assignedChunks = 0;
        ALLOCATE_ALGORITHM(_chunkHint, _chunkAmount)

        assignedChunks = 0;
        ALLOCATE_ALGORITHM(0, _chunkHint)
    }

    return nullptr;
}

#undef ALLOCATE_ALGORITHM

inline bool MemoryPool::_deallocate(void* p, size_t bytes)
{
    if (p >= _memory && p < _memory + _poolSize)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        const size_t requiredChunks = calcChunkAmount(bytes);
        const size_t chunkIndex = (reinterpret_cast<size_t>(p) - reinterpret_cast<size_t>(_memory)) / _chunkSize;
        _chunkHint = chunkIndex;
        std::fill(_memoryMap + chunkIndex, _memoryMap + chunkIndex + requiredChunks, false);
        return true;
    }

    return false;
}

size_t ParallelMemoryPool::_poolAmount = 0;
MemoryPool* ParallelMemoryPool::_memoryPools = nullptr;

void ParallelMemoryPool::setUp(size_t poolSize, size_t chunkSize, size_t poolAmount)
{
    assert(_memoryPools == nullptr);

    _poolAmount = poolAmount;
    _memoryPools = static_cast<MemoryPool*>(::operator new(AlignPool::memorySize(poolAmount)));

    for (size_t i = 0; i < _poolAmount; ++i)
        ::new((void*) AlignPool::item(_memoryPools, i)) MemoryPool(poolSize, chunkSize);
}

void ParallelMemoryPool::tearDown()
{
    assert(_memoryPools);

    for (size_t i = 0; i < _poolAmount; ++i)
        AlignPool::item(_memoryPools, i)->~MemoryPool();

    ::operator delete((void*) _memoryPools);
    _memoryPools = nullptr;
}

void* ParallelMemoryPool::_allocate(size_t bytes)
{
    assert(_memoryPools);

    static __thread MemoryPool* const preferedMemoryPool = AlignPool::item(_memoryPools,
        std::hash<std::thread::id>()(std::this_thread::get_id()) % _poolAmount);

    if (void* memory = preferedMemoryPool->_allocate(bytes))
        return memory;

    for (size_t i = 0; i < _poolAmount; ++i)
        if (void* memory = AlignPool::item(_memoryPools, i)->_allocate(bytes))
            return memory;

#ifndef NDEBUG
    std::cout << "allocating using new!\n";
#endif

    return ::operator new(bytes);
}

void ParallelMemoryPool::_deallocate(void* p, size_t bytes)
{
    assert(_memoryPools);

    for (size_t i = 0; i < _poolAmount; ++i)
        if (AlignPool::item(_memoryPools, i)->_deallocate(p, bytes))
            return;

    ::operator delete(p);
}

}
}
