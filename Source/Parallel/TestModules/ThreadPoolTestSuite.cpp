#include <Parallel/ThreadPool.hpp>
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

class ThreadPoolTestSuite : public Test
{
public:
    void TearDown() override
    {
        sut = nullptr;

        ASSERT_THAT(count, Eq(expectedCount));
    }

    void incrementCounter()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        count++;
    }

    void incrementCounterTwoTimes(ThreadPool* pool)
    {
        incrementCounter();
        pool->queueTask(std::bind(&ThreadPoolTestSuite::incrementCounter, this));
    }

    uint32_t getCount(uint32_t expectedCount)
    {
        auto start = std::chrono::high_resolution_clock::now();

        while (count != expectedCount && start + std::chrono::seconds(1) > std::chrono::high_resolution_clock::now())
            std::this_thread::yield();

        return count;
    }

    uint32_t expectedCount{0};
    std::atomic<uint32_t> count{0};
    std::unique_ptr<ThreadPool> sut{new ThreadPool};
};

TEST_F(ThreadPoolTestSuite, shouldExecuteAllTasks)
{
    expectedCount = 10;
    for (size_t i = 0; i < expectedCount; ++i)
        sut->queueTask(std::bind(&ThreadPoolTestSuite::incrementCounter, this));
};

TEST_F(ThreadPoolTestSuite, shouldQueueMultipleTasksAtOnce)
{
    expectedCount = 10;
    std::vector<ThreadPoolTask> tasks;
    for (size_t i = 0; i < expectedCount; ++i)
        tasks.push_back(std::bind(&ThreadPoolTestSuite::incrementCounter, this));
    sut->queueTasks(std::move(tasks));
}

TEST_F(ThreadPoolTestSuite, shouldWorkWhenThreadPushedMoreTasks)
{
    expectedCount = 20;
    for (size_t i = 0; i < expectedCount / 2; ++i)
        sut->queueTask(std::bind(&ThreadPoolTestSuite::incrementCounterTwoTimes, this, sut.get()));
}

TEST_F(ThreadPoolTestSuite, pushingEmptyTaskShouldDoNothing)
{
    expectedCount = 2;
    sut->queueTask(std::bind(&ThreadPoolTestSuite::incrementCounter, this));
    sut->queueTask(ThreadPoolTask());

    std::vector<ThreadPoolTask> tasks;
    tasks.push_back(std::bind(&ThreadPoolTestSuite::incrementCounter, this));
    tasks.push_back(ThreadPoolTask());
    sut->queueTasks(tasks);
}

TEST_F(ThreadPoolTestSuite, shouldQueueTaskWithArguments)
{
    expectedCount = 3;
    sut->queueTask(std::mem_fn(&ThreadPoolTestSuite::incrementCounter), this);
    sut->queueTask(std::mem_fn(&ThreadPoolTestSuite::incrementCounterTwoTimes), this, sut.get());
}

TEST_F(ThreadPoolTestSuite, shouldQueueFutures)
{
    expectedCount = 10;
    for (size_t i = 0; i < expectedCount; ++i)
        sut->queueTask(std::bind(&ThreadPoolTestSuite::incrementCounter, this));

    std::future<uint32_t> fut = sut->queueFuture(std::mem_fn(&ThreadPoolTestSuite::getCount), this, expectedCount);
    ASSERT_THAT(fut.get(), Eq(expectedCount));
}

}
}
}
