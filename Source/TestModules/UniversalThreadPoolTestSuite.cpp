#include <UniversalThreadPool.hpp>
#include <TestModules/MockFunction.hpp>
#include <gmock/gmock.h>

namespace Common
{
namespace Ut
{

using namespace testing;
using Common::MockFunction;

class UniversalThreadPoolTestSuite : public Test
{
public:
    static constexpr size_t TASK_AMOUNT = 22;

    UniversalThreadPoolTestSuite()
        : sut(TASK_AMOUNT / 2)
    {
    }

    std::function<void ()> createTask(StrictMock<MockFunction<void ()>>& mock, std::chrono::milliseconds t)
    {
        EXPECT_CALL(mock, Call());

        return [&mock, t]
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(t));
            mock.Call();
        };
    }

    std::array<StrictMock<MockFunction<void ()>>, TASK_AMOUNT> mocks;

    UniversalThreadPool sut;
};

TEST_F(UniversalThreadPoolTestSuite, isSynchronizedAfterIsCreated)
{
    ASSERT_THAT(sut.isSynchronized(), Eq(true));
}

TEST_F(UniversalThreadPoolTestSuite, testQueuingSingleTasks)
{
    for (auto& mock : mocks)
        sut.queueTask(createTask(mock, std::chrono::milliseconds(50)));
    sut.synchronize();
}

TEST_F(UniversalThreadPoolTestSuite, testQueuingSingleTasksOneAfterAnother)
{
    for (auto& mock : mocks)
    {
        sut.queueTask(createTask(mock, std::chrono::milliseconds(10)));
        sut.synchronize();
    }
}

TEST_F(UniversalThreadPoolTestSuite, testQueuingMultipleTask)
{
    std::vector<std::function<void ()>> tasks;
    for (auto& mock : mocks)
        tasks.push_back(createTask(mock, std::chrono::milliseconds(50)));
    sut.queueTasks(tasks);
    ASSERT_THAT(sut.isSynchronized(), Eq(false));
    sut.synchronize();
    ASSERT_THAT(sut.isSynchronized(), Eq(true));
}

}
}
