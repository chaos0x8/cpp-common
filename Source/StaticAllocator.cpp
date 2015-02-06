#include <StaticAllocator.hpp>
#include <stdexcept>
#include <cassert>

namespace Common
{
namespace Detail
{

size_t StaticAllocatorBase::_staticPoolSize = 0;
size_t StaticAllocatorBase::_chunkSize = 0;
size_t StaticAllocatorBase::_chunkIndexHint;
size_t StaticAllocatorBase::_totalChunkAmount;
void* StaticAllocatorBase::_memory = nullptr;
std::vector<char> StaticAllocatorBase::_memoryMap;
std::mutex StaticAllocatorBase::_mutex;

void StaticAllocatorBase::setUp(size_t staticPoolSize, size_t chunkSize)
{
    assert(staticPoolSize && chunkSize);
    assert(staticPoolSize % chunkSize == 0);

    _memory = ::operator new(staticPoolSize);

    _staticPoolSize = staticPoolSize;
    _chunkSize = chunkSize;
    _chunkIndexHint = 0;
    _totalChunkAmount = _staticPoolSize / _chunkSize;
    _memoryMap.resize(_totalChunkAmount, 0);
}

void StaticAllocatorBase::tearDown()
{
    ::operator delete(_memory);
    _memory = nullptr;
}

#define ALLOCATION_ALGORITH(startIndex, endIndex) \
    for (size_t i = startIndex; i < endIndex; ++i) \
    { \
        if (_memoryMap[i] == 0) \
        { \
            if (++assignedChunks == requiredChunks) \
            { \
                size_t firstAssignedChunkIndex = (i + 1u) - requiredChunks; \
                for (size_t k = 0; k < requiredChunks; ++k) \
                    _memoryMap[firstAssignedChunkIndex + k] = 1; \
                _chunkIndexHint = firstAssignedChunkIndex + requiredChunks; \
                return _memory + (firstAssignedChunkIndex * _chunkSize); \
            } \
        } \
        else \
        { \
            assignedChunks = 0; \
        } \
    }

void* StaticAllocatorBase::_allocate(size_t bytes)
{
    assert(_memory);

    const size_t requiredChunks = calcChunkAmount(bytes);

    if (bytes < _staticPoolSize)
    {
        size_t assignedChunks = 0;

        std::unique_lock<std::mutex> lock(_mutex);

        ALLOCATION_ALGORITH(_chunkIndexHint, _totalChunkAmount)
        assignedChunks = 0;
        ALLOCATION_ALGORITH(0, _chunkIndexHint)
    }

    return ::operator new(bytes);
}

#undef ALLOCATION_ALGORITH

void StaticAllocatorBase::_deallocate(void* p, size_t bytes)
{
    assert(_memory);

    if (p >= _memory && p < _memory + _staticPoolSize)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        const size_t requiredChunks = calcChunkAmount(bytes);
        const size_t chunkIndex = (reinterpret_cast<size_t>(p) - reinterpret_cast<size_t>(_memory)) / _chunkSize;
        for (size_t k = 0; k < requiredChunks; ++k)
            _memoryMap[chunkIndex + k] = 0;
        _chunkIndexHint = chunkIndex;
        return;
    }

    ::operator delete(p);
}

}
}