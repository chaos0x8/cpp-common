#include <IndependentProcessing.hpp>
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

}
}

