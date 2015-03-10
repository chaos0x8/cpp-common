#pragma once

#include <functional>
#include <gmock/gmock.h>

namespace Common
{
namespace Detail
{

using std::placeholders::_1;
using std::placeholders::_2;

template <typename T>
struct TypeWrapper
{
    typedef T type;
    typedef std::function<T> function_type;
};

template <typename T>
struct TypeWrapper<std::function<T>>
{
    typedef T type;
    typedef std::function<T> function_type;
};

template <typename R>
inline std::function<R ()> bindMockFunction(testing::MockFunction<R ()>& mock)
{
    return std::bind(&testing::MockFunction<R ()>::Call, &mock);
}

template <typename R, typename A0>
inline std::function<R (A0)> bindMockFunction(testing::MockFunction<R (A0)>& mock)
{
    return std::bind(&testing::MockFunction<R (A0)>::Call, &mock, _1);
}

template <typename R, typename A0, typename A1>
inline std::function<R (A0, A1)> bindMockFunction(testing::MockFunction<R (A0, A1)>& mock)
{
    return std::bind(&testing::MockFunction<R (A0, A1)>::Call, &mock, _1, _2);
}

}

template <typename T>
class MockFunction : public testing::MockFunction<typename Detail::TypeWrapper<T>::type>
{
public:
    operator typename Detail::TypeWrapper<T>::function_type()
    {
        return Detail::bindMockFunction(*this);
    }
};

}
