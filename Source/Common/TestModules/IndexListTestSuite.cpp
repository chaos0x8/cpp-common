#include <Common/IndexList.hpp>
#include <tuple>
#include <string>
#include <gmock/gmock.h>

namespace Common
{
namespace Ut
{

using namespace testing;

class FunctionStub
{
public:
    template <typename... Args>
    static int f1(Args... args)
    {
        ++f1Count;
        return 0;
    }

    template <typename U, int... T>
    static void callF1(U values, IndexList<T...>)
    {
        f1(std::get<T>(values)...);
    }

    template <typename U, int... T>
    static void callF2(U values, IndexList<T...>)
    {
        int dummy[sizeof...(T)] = { f1(std::get<T>(values))... };
    }

    static size_t f1Count;
};

size_t FunctionStub::f1Count = 0;

class GenerateIndexListTestSuite : public Test
{
public:
    ~GenerateIndexListTestSuite()
    {
        FunctionStub::f1Count = 0;
    }

    std::tuple<int, std::string, float> values;
};

TEST_F(GenerateIndexListTestSuite, shouldCallFunctionF1)
{
    FunctionStub::callF1(values, GenerateIndexList<3>::type());
    ASSERT_THAT(FunctionStub::f1Count, Eq(1u));
}

TEST_F(GenerateIndexListTestSuite, shouldCallFunctionF2)
{
    FunctionStub::callF2(values, GenerateIndexList<3>::type());
    ASSERT_THAT(FunctionStub::f1Count, Eq(3u));
}

}
}
