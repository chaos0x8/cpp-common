#include <StaticAllocator.hpp>

namespace Common
{

StaticMemory::StaticMemory(std::size_t size)
    : size(size)
{
    location = 0;
}

StaticMemory::~StaticMemory()
{
    ::operator delete(memory);
}

void StaticMemory::init()
{
    if (!memory)
        memory = ::operator new(size);
}

void* StaticMemory::allocate(std::size_t s)
{
    if (!memory)
        memory = ::operator new(size);

    void* result = memory + location.fetch_add(s);

    if (result + s > memory + size)
        throw std::bad_alloc();

    return result;
}

void StaticMemory::deallocate(void* p, std::size_t size)
{

}

}
