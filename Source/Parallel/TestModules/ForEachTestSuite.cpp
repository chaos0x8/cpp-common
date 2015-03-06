#include <Parallel/ForEach.hpp>
#include <gmock/gmock.h>
#include <chrono>

namespace Common
{
namespace Parallel
{

using namespace testing;

class ForEachTestData
{
public:
    void operation()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        executed = true;
    }

    bool executed{false};
    int padding{0};
};

class ForEachTestSuite : public Test
{
public:
    void assertAllTestData(bool expectedValue) const
    {
        ASSERT_THAT(std::all_of(std::begin(testData), std::end(testData), [=] (const ForEachTestData& x) { return x.executed == expectedValue; }), Eq(true));
    }
    void assertAnyOfTestData(bool expectedValue) const
    {
        ASSERT_THAT(std::any_of(std::begin(testData), std::end(testData), [=] (const ForEachTestData& x) { return x.executed == expectedValue; }), Eq(true));
    }

    std::vector<ForEachTestData> testData{375};
};

TEST_F(ForEachTestSuite, shouldExecuteAll)
{
    forEach(testData, std::mem_fn(&ForEachTestData::operation), 24);
    assertAllTestData(true);
}

TEST_F(ForEachTestSuite, shouldHoldExecutionContext)
{
    auto context = forEach(testData, std::mem_fn(&ForEachTestData::operation));
    assertAnyOfTestData(false);
    context->wait();
    assertAnyOfTestData(true);
}

}
}
