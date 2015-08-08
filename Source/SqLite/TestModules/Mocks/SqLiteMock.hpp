#pragma once

#include <SqLite/ISqLite.hpp>
#include <gmock/gmock.h>

namespace Common
{
namespace SqLite
{
namespace UT
{

class SqLiteMock : public ISqLite
{
public:
    MOCK_METHOD1(open, void(const std::string&));
    MOCK_METHOD1(select, SelectResult(const std::string&));
    MOCK_METHOD1(execute, void(const std::string&));
    MOCK_METHOD0(getLastInsertedId, int64_t());

    MOCK_METHOD1(transaction, bool(std::function<void ()>));

    MOCK_METHOD0(setTestMode, void());

    operator bool() const override
    {
        return operatorBool();
    }

    MOCK_CONST_METHOD0(operatorBool, bool());
};

}
}
}
