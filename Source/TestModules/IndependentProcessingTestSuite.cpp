#include <IndependentProcessing.hpp>
#include <StaticAllocator.hpp>
#include <boost/optional.hpp>
#include <gmock/gmock.h>

namespace Common
{
namespace Ut
{

using namespace testing;

class IndependentTaskTestSuite : public Test
{
public:
    std::vector<int> inputData = { 10, 42, 70, 105, 17 };
};

TEST_F(IndependentTaskTestSuite, shouldProccessData)
{
    auto cut = runIndependentProcessing(inputData, [](int& x) { x += 100; }, 3u);
    cut->join();

    ASSERT_THAT(inputData, ElementsAre(110, 142, 170, 205, 117));
}

TEST_F(IndependentTaskTestSuite, shouldProccessDataAndGiveReuslt)
{
    auto cut = runIndependentProcessing(inputData, [](int x) { return x + 100; }, 17u);
    ASSERT_THAT(cut->get(), UnorderedElementsAre(110, 142, 170, 205, 117));
    ASSERT_THAT(inputData, ElementsAre(10, 42, 70, 105, 17));
}

TEST_F(IndependentTaskTestSuite, shouldProccessDataAndGiveOptionalReuslt)
{
    auto cut = runIndependentProcessing(inputData, [](int x) -> boost::optional<int>
    {
        if (x % 2 == 0)
            return x + 100;
        return boost::none;
    });
    ASSERT_THAT(cut->get(), UnorderedElementsAre(110, 142, 170));
    ASSERT_THAT(inputData, ElementsAre(10, 42, 70, 105, 17));
}

template <typename T>
using vector = std::vector<T, StaticAllocator<T>>;

TEST_F(IndependentTaskTestSuite, shouldUseDesiredAllocator)
{
    StaticAllocatorInitializer init(1024 * std::thread::hardware_concurrency(), 256);

    auto cut = runIndependentProcessing<StaticAllocator>(inputData, [](int x) { return x + 100; });
    const vector<int> result = cut->get();
    ASSERT_THAT(result, UnorderedElementsAre(110, 142, 170, 205, 117));
    ASSERT_THAT(inputData, ElementsAre(10, 42, 70, 105, 17));
}

class IndependentProcessingStressTestSuite : public Test
{
public:
    IndependentProcessingStressTestSuite()
    {
        for (size_t i = 0; i < 1024*1024; ++i)
            input.push_back(i);
    }

    std::vector<int> input;
};

TEST_F(IndependentProcessingStressTestSuite, DISABLED_stressWithoutOutput)
{
    runIndependentProcessing(std::move(input), [](int x) -> void { std::to_string(x); });
}

TEST_F(IndependentProcessingStressTestSuite, DISABLED_stressWithOutput)
{
    runIndependentProcessing(std::move(input), [](int x) -> std::string { return std::to_string(x); });
}

TEST_F(IndependentProcessingStressTestSuite, DISABLED_stressWithOptionalOutput)
{
    auto cut = runIndependentProcessing(std::move(input),
    [](int x) -> boost::optional<std::string>
    {
        if (x % 2 == 0)
            return boost::none;
        return std::to_string(x);
    });

    const size_t expectedSize = std::count_if(std::begin(input), std::end(input), [](int x) { return x % 2 != 0; });
    ASSERT_THAT(cut->get(), SizeIs(expectedSize));
}

}
}

