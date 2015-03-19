/*!
    \author <https://github.com/chaos0x8>
    \copyright
    Copyright (c) 2015, <https://github.com/chaos0x8>

    \copyright
    Permission to use, copy, modify, and/or distribute this software for any
    purpose with or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    \copyright
    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <Common/StaticAllocator.hpp>
#include <thread>
#include <array>
#include <gmock/gmock.h>

extern size_t MEM_SIZE;

namespace Common
{
namespace Ut
{

using namespace testing;

TEST(StaticAllocatorTestSuite, shouldNotOverflowAfterStaticMemoryIsDrained)
{
    std::vector<int, StaticAllocator<int>> vec;
    vec.resize(MEM_SIZE / sizeof(int));
}

TEST(StaticAllocatorTestSuite, shuldBeZeroInitialized)
{
    std::vector<int, StaticAllocator<int>> vec;
    vec.resize(MEM_SIZE / sizeof(int) / std::thread::hardware_concurrency());

    for (int& x : vec)
    {
        ASSERT_THAT(x, Eq(int()));
    }
}

TEST(StaticAllocatorTestSuite, shuldCreateElementsWithValidValue)
{
    std::vector<int, StaticAllocator<int>> vec;
    vec.push_back(15);
    vec.emplace_back(42);
    vec.push_back(17);
    ASSERT_THAT(vec, ElementsAre(15, 42, 17));
}

TEST(StaticAllocatorTestSuite, shouldBeUsableInSharedPointer)
{
    struct Foo
    {
        Foo(int& value) : value(value) { ++value; }
        ~Foo() { ++value; }

        int& value;
    };

    int value = 0;
    std::shared_ptr<Foo> foo = std::allocate_shared<Foo>(Common::StaticAllocator<Foo>(), value);
    ASSERT_THAT(value, Eq(1));
    foo = nullptr;
    ASSERT_THAT(value, Eq(2));
}

TEST(StaticAllocatorTestSuite, shouldBeUsableInString)
{
    using _string = std::basic_string<char, std::char_traits<char>, Common::StaticAllocator<char>>;
    _string test = "Hello world!";
    ASSERT_THAT(test, Eq("Hello world!"));
}

TEST(StaticAllocatorTestSuite, memoryShouldBeAligned)
{
    Common::StaticAllocator<int> sut;
    int* p = sut.allocate(3);

    ASSERT_THAT(reinterpret_cast<size_t>(p) % 64ul, Eq(0ul));

    sut.deallocate(p, 3);
}

#ifdef NDEBUG
class StaticAllocatorStressTestSuite : public Test
{
public:
    template <typename string>
    void stringPerformanceTest()
    {
        for (std::thread& t : threads)
        {
            t = std::thread([]
            {
                for (size_t i = 0; i < 128; ++i)
                {
                    string x = "hi";
                    for (size_t k = 0; k < 1024 * 1024; ++k)
                        x += "x";
                }
            });
        }
        std::for_each(std::begin(threads), std::end(threads), std::mem_fn(&std::thread::join));
    }

    std::array<std::thread, 4> threads;
};

TEST_F(StaticAllocatorStressTestSuite, DISABLED_performanceHackString)
{
    using string = std::basic_string<char, std::char_traits<char>, Common::StaticAllocator<char>>;

    stringPerformanceTest<string>();
}

TEST_F(StaticAllocatorStressTestSuite, DISABLED_performanceString)
{
    stringPerformanceTest<std::string>();
}
#endif

}
}
