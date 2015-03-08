#include <Parallel/ScopedThread.hpp>
#include <gmock/gmock.h>
#include <atomic>
#include <chrono>

namespace Common
{
namespace Parallel
{
namespace UT
{

using namespace testing;

class TestObject
{
public:
    void operation()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        count++;
    }

    std::atomic<uint32_t> count{0};
};

class ScopedThreadTestSuite : public Test
{
public:
    void TearDown() override
    {
        ASSERT_THAT(obj.count, Eq(expectedCount));
    }

    TestObject obj;
    uint32_t expectedCount{0};
};

TEST_F(ScopedThreadTestSuite, itsSaveToNotJoin)
{
    expectedCount = 1;

    ScopedThread t(std::mem_fn(&TestObject::operation), &obj);
}

TEST_F(ScopedThreadTestSuite, doubleJoinIsPossible)
{
    expectedCount = 1;

    ScopedThread t1(std::mem_fn(&TestObject::operation), &obj);
    t1.join();
    t1.join();
}

TEST_F(ScopedThreadTestSuite, easyToStoreInContainers)
{
    expectedCount = 5;

    std::vector<ScopedThread> threads;
    for (size_t i = 0; i < expectedCount; ++i)
        threads.emplace_back(std::mem_fn(&TestObject::operation), &obj);
}

}
}
}
