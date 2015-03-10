#include <Common/StaticAllocator.hpp>
#include <thread>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

size_t MEM_SIZE = 1024ul * 1024ul;

void initMemory() __attribute__((constructor));
void initMemory()
{
    Common::Detail::ParallelMemoryPool::setUp(MEM_SIZE / std::thread::hardware_concurrency(), 1024ul, std::thread::hardware_concurrency());
}

void destroyMemory() __attribute__((destructor));
void destroyMemory()
{
    Common::Detail::ParallelMemoryPool::tearDown();
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

