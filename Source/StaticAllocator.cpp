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

MemoryPool::MemoryPool(size_t poolSize, size_t chunkSize)
    : _poolSize(poolSize),
      _chunkSize(chunkSize),
      _chunkAmount(poolSize / chunkSize)
{
    assert(_chunkAmount);
    assert(_poolSize % _chunkSize == 0);

    const size_t extraMemory = sizeof(std::mutex) + sizeof(size_t) + sizeof(bool) * _chunkAmount;
    const size_t extraChunks = calcChunkAmount(extraMemory);

    _heap = ::operator new(extraChunks * _chunkSize + _poolSize);
    _mutex = static_cast<std::mutex*>(_heap);
    _chunkHint = static_cast<size_t*>(_heap + sizeof(std::mutex));
    _memoryMap = static_cast<bool*>(_heap + sizeof(std::mutex) + sizeof(size_t));
    _memory = _heap + _chunkSize * extraChunks;

    ::new((void*) _mutex) std::mutex();
    ::new((void*) _chunkHint) size_t(0);
    std::fill(_memoryMap, _memoryMap + _chunkAmount, false);
}

MemoryPool::~MemoryPool()
{
    _mutex->~mutex();
    _chunkHint->~size_t();
    ::operator delete(_heap);
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
                *_chunkHint = i + 1u; \
                size_t firstAssignedChunkIndex = (i + 1u) - requiredChunks; \
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
    if (_mutex->try_lock())
    {
        std::unique_lock<std::mutex> lock(*_mutex, std::adopt_lock);

        size_t requiredChunks = calcChunkAmount(bytes);

        size_t assignedChunks = 0;
        ALLOCATE_ALGORITHM(*_chunkHint, _chunkAmount)

        assignedChunks = 0;
        ALLOCATE_ALGORITHM(0, *_chunkHint)
    }

    return nullptr;
}

#undef ALLOCATE_ALGORITHM

inline bool MemoryPool::_deallocate(void* p, size_t bytes)
{
    if (p >= _memory && p < _memory + _poolSize)
    {
        std::unique_lock<std::mutex> lock(*_mutex);
        const size_t requiredChunks = calcChunkAmount(bytes);
        const size_t chunkIndex = (reinterpret_cast<size_t>(p) - reinterpret_cast<size_t>(_memory)) / _chunkSize;
        *_chunkHint = chunkIndex;
        std::fill(_memoryMap + chunkIndex, _memoryMap + chunkIndex + requiredChunks, false);
        return true;
    }

    return false;
}

size_t* ParallelMemoryPool::_poolAmount = nullptr;
MemoryPool* ParallelMemoryPool::_memoryPools = nullptr;

void ParallelMemoryPool::setUp(size_t poolSize, size_t chunkSize, size_t poolAmount)
{
    assert(_poolAmount == nullptr);
    assert(_memoryPools == nullptr);

    void* heap = ::operator new(sizeof(size_t) + poolAmount * sizeof(MemoryPool));
    _poolAmount = static_cast<size_t*>(heap);
    _memoryPools = static_cast<MemoryPool*>(heap + sizeof(size_t));

    ::new((void*) _poolAmount) size_t(poolAmount);
    for (size_t i = 0; i < *_poolAmount; ++i)
        ::new((void*) &_memoryPools[i]) MemoryPool(poolSize, chunkSize);
}

void ParallelMemoryPool::tearDown()
{
    assert(_poolAmount);
    assert(_memoryPools);

    for (size_t i = 0; i < *_poolAmount; ++i)
        _memoryPools[i].~MemoryPool();
    _poolAmount->~size_t();

    ::operator delete((void*) _poolAmount);

    _poolAmount = nullptr;
    _memoryPools = nullptr;
}

void* ParallelMemoryPool::_allocate(size_t bytes)
{
    assert(_poolAmount);
    assert(_memoryPools);

    std::hash<std::thread::id> hash;
    const size_t preferedPoolId = hash(std::this_thread::get_id()) % *_poolAmount;

    if (void* memory = _memoryPools[preferedPoolId]._allocate(bytes))
        return memory;

    for (size_t i = 0; i < *_poolAmount; ++i)
        if (void* memory = _memoryPools[i]._allocate(bytes))
            return memory;

#ifndef NDEBUG
    std::cout << "allocating using new!\n";
#endif

    return ::operator new(bytes);
}

#undef POOL_SELECTION_ALGORITHM

void ParallelMemoryPool::_deallocate(void* p, size_t bytes)
{
    assert(_poolAmount);
    assert(_memoryPools);

    for (size_t i = 0; i < *_poolAmount; ++i)
        if (_memoryPools[i]._deallocate(p, bytes))
            return;

    ::operator delete(p);
}

}
}
