#include <Parallel/Compute.hpp>
#include <gmock/gmock.h>
#include <chrono>

namespace Common
{
namespace Parallel
{

using namespace testing;

class ComputeTestData
{
public:
    int operation()
    {
        return 42;
    }

    int padding{0};
};

class ComputeTestSuite : public Test
{
public:
    void assertAllTestData(const std::vector<int>& data, int expectedValue) const
    {
        ASSERT_THAT(std::all_of(std::begin(data), std::end(data), [=] (const int& x) { return x == expectedValue; }), Eq(true));
    }

    std::vector<ComputeTestData> testData{375};
};

TEST_F(ComputeTestSuite, shouldExecuteAll)
{
    auto result = compute(testData, std::mem_fn(&ComputeTestData::operation), 24)->get();
    assertAllTestData(result, 42);
}

TEST_F(ComputeTestSuite, shouldHoldExecutionContext)
{
    auto context = compute(testData, std::mem_fn(&ComputeTestData::operation));
    context->wait();
    assertAllTestData(context->get(), 42);
}

}
}

