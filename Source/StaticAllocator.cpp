#include <StaticAllocator.hpp>
#include <stdexcept>

namespace Common
{
namespace Detail
{

size_t StaticAllocatorBase::_staticPoolSize = 0;
size_t StaticAllocatorBase::_chunkSize = 0;
void* StaticAllocatorBase::_memory = nullptr;
void* StaticAllocatorBase::_position = nullptr;
std::vector<char> StaticAllocatorBase::_memoryMap;
std::mutex StaticAllocatorBase::_mutex;

void StaticAllocatorBase::setUp(size_t staticPoolSize, size_t chunkSize)
{
    _memory = ::operator new(staticPoolSize);
    _position = _memory;

    _staticPoolSize = staticPoolSize;
    _chunkSize = chunkSize;
    _memoryMap.resize(calcTotalChunkAmount(), 0);
}

void StaticAllocatorBase::tearDown()
{
    ::operator delete(_memory);
    _memory = nullptr;
    _position = nullptr;
    _staticPoolSize = 0;
    _chunkSize = 0;
}

void* StaticAllocatorBase::_allocate(size_t bytes, void* hint)
{
    const size_t requiredChunks = calcChunkAmount(bytes);

    if (bytes < _staticPoolSize)
    {
        size_t assignedChunks = 0;
        std::unique_lock<std::mutex> lock(_mutex);
        for (size_t i = 0; i < calcTotalChunkAmount(); ++i)
        {
            if (_memoryMap[i] == 0)
            {
                if (++assignedChunks == requiredChunks)
                {
                    size_t firstAssignedChunkIndex = (i + 1u) - requiredChunks;
                    for (size_t k = 0; k < requiredChunks; ++k)
                        _memoryMap[firstAssignedChunkIndex + k] = 1;
                    return _memory + (firstAssignedChunkIndex * _chunkSize);
                }
            }
            else
            {
                assignedChunks = 0;
            }
        }
    }

    return ::operator new(bytes);
}

void StaticAllocatorBase::_deallocate(void* p, size_t bytes)
{
    if (p >= _memory && p < _memory + _staticPoolSize)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        const size_t requiredChunks = calcChunkAmount(bytes);
        const size_t chunkIndex = (reinterpret_cast<size_t>(p) - reinterpret_cast<size_t>(_memory)) / _chunkSize;
        for (size_t k = 0; k < requiredChunks; ++k)
            _memoryMap[chunkIndex + k] = 0;
        return;
    }

    ::operator delete(p);
}

}
}
