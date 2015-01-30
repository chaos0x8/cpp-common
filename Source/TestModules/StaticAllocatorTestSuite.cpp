#include <StaticAllocator.hpp>
#include <gmock/gmock.h>

namespace Common
{
namespace Ut
{

using namespace testing;

class StaticMemoryTestSuite : public Test
{
public:
    StaticMemoryTestSuite()
        : sut(MEM_SIZE)
    {
    }

    static constexpr size_t MEM_SIZE = 1024u * 1024u;

    StaticMemory sut;
};

TEST_F(StaticMemoryTestSuite, shouldCreateAndDestroy)
{
    StaticMemory x(1024);
}

TEST_F(StaticMemoryTestSuite, shouldAllocateMemory)
{
    void* m = sut.allocate(1);
    ASSERT_THAT(m, Ne(nullptr));
    ASSERT_THAT(sut.allocate(4), Eq(m + 1));
    ASSERT_THAT(sut.allocate(4), Eq(m + 5));
}

TEST_F(StaticMemoryTestSuite, shouldThrowWhenOutOfMemory)
{
    int* mem = (int*) sut.allocate(MEM_SIZE);
    ASSERT_THROW(sut.allocate(1), std::bad_alloc);
}

TEST_F(StaticMemoryTestSuite, shouldThrowWhenOutOfMemory2)
{
    ASSERT_THROW(sut.allocate(MEM_SIZE + 1), std::bad_alloc);
}

class StaticAllocatorTestSuite : public Test
{
public:
    StaticAllocatorTestSuite()
        : mem(std::make_shared<StaticMemory>(MEM_SIZE)),
          sut(mem)
    {
    }

    static constexpr size_t MEM_SIZE = 1024u;

    std::shared_ptr<StaticMemory> mem;
    StaticAllocator<int> sut;
};

constexpr size_t StaticAllocatorTestSuite::MEM_SIZE;

TEST_F(StaticAllocatorTestSuite, shouldOverflowInVector)
{
    std::vector<int, StaticAllocator<int>> vec(sut);
    vec.resize(MEM_SIZE / sizeof(int));
    ASSERT_THROW(vec.push_back(42), std::bad_alloc);
}

TEST_F(StaticAllocatorTestSuite, shuldBeZeroInitialized)
{
    std::vector<int, StaticAllocator<int>> vec(sut);
    vec.resize(MEM_SIZE / sizeof(int));

    for (int& x : vec)
    {
        ASSERT_THAT(x, Eq(int()));
    }
}

TEST_F(StaticAllocatorTestSuite, shuldCreateElementsWithValidValue)
{
    std::vector<int, StaticAllocator<int>> vec(sut);
    vec.push_back(15);
    vec.emplace_back(42);
    vec.push_back(17);
    ASSERT_THAT(vec, ElementsAre(15, 42, 17));
}

TEST_F(StaticAllocatorTestSuite, shouldBeUsableInSharedPointer)
{
    std::shared_ptr<int> x = std::allocate_shared<int>(sut, 42);
    ASSERT_THAT(*x, Eq(42));
}

}
}
