#include <AutoThread.hpp>
#include <TestModules/MockFunction.hpp>
#include <gmock/gmock.h>

namespace Common
{

using namespace testing;

class AutoThreadTestSuite : public Test
{
public:
    void method(StrictMock<MockFunction<void ()>>& mock)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
        mock.Call();
    }

    StrictMock<MockFunction<void ()>> mock1;
    StrictMock<MockFunction<void ()>> mock2;

        AutoThread sut;
};

TEST_F(AutoThreadTestSuite, shouldBeSafeToNotJoin)
{
    EXPECT_CALL(mock1, Call()).Times(2);
    EXPECT_CALL(mock2, Call());
    sut = std::thread(&AutoThreadTestSuite::method, this, std::ref(mock1));
    sut = std::thread(&AutoThreadTestSuite::method, this, std::ref(mock2));
    sut = std::thread(&AutoThreadTestSuite::method, this, std::ref(mock1));
}

}
