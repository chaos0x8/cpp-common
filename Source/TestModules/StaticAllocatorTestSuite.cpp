#include <StaticAllocator.hpp>
#include <gmock/gmock.h>

namespace Common
{
namespace Ut
{

using namespace testing;

class StaticAllocatorTestSuite : public Test
{
public:
    StaticAllocatorTestSuite()
        : staticAlloc(MEM_SIZE, 64)
    {
    }

    static constexpr size_t MEM_SIZE = 1024u;

    StaticAllocatorInitializer staticAlloc;
};

constexpr size_t StaticAllocatorTestSuite::MEM_SIZE;

TEST_F(StaticAllocatorTestSuite, shouldNotOverflowAfterStaticMemoryIsDrained)
{
    std::vector<int, StaticAllocator<int>> vec;
    vec.resize(MEM_SIZE / sizeof(int));
    vec.push_back(42);
}

TEST_F(StaticAllocatorTestSuite, shuldBeZeroInitialized)
{
    std::vector<int, StaticAllocator<int>> vec;
    vec.resize(MEM_SIZE / sizeof(int));

    for (int& x : vec)
    {
        ASSERT_THAT(x, Eq(int()));
    }
}

TEST_F(StaticAllocatorTestSuite, shuldCreateElementsWithValidValue)
{
    std::vector<int, StaticAllocator<int>> vec;
    vec.push_back(15);
    vec.emplace_back(42);
    vec.push_back(17);
    ASSERT_THAT(vec, ElementsAre(15, 42, 17));
}

TEST_F(StaticAllocatorTestSuite, shouldBeUsableInSharedPointer)
{
    std::shared_ptr<int> x = std::allocate_shared<int>(Common::StaticAllocator<int>(), 42);
    ASSERT_THAT(*x, Eq(42));
}

TEST_F(StaticAllocatorTestSuite, shouldBeUsableInString)
{
    using _string = std::basic_string<char, std::char_traits<char>, Common::StaticAllocator<char>>;
    _string test = "Hello world!";
    ASSERT_THAT(test, Eq("Hello world!"));
}

}
}
