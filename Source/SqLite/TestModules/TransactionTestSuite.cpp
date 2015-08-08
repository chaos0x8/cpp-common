#include <SqLite/Transaction.hpp>
#include <SqLite/TestModules/Mocks/SqLiteMock.hpp>
#include <gmock/gmock.h>

namespace Common
{
namespace SqLite
{
namespace UT
{

using namespace testing;

class TransactionTestSuite : public Test
{
public:
    StrictMock<SqLiteMock> sqLite;
};

TEST_F(TransactionTestSuite, successfulTransaction)
{
    InSequence dummy;
    EXPECT_CALL(sqLite, execute("begin transaction"));
    EXPECT_CALL(sqLite, execute("commit transaction"));

    {
        Transaction sut(sqLite);
        sut.commit();
    }
}

TEST_F(TransactionTestSuite, rollback)
{
    InSequence dummy;
    EXPECT_CALL(sqLite, execute("begin transaction"));
    EXPECT_CALL(sqLite, execute("rollback"));

    {
        Transaction sut(sqLite);
    }
}

}
}
}
