/*!
 *  \author <https://github.com/chaos0x8>
 *  \copyright
 *  Copyright (c) 2015 - 2016, <https://github.com/chaos0x8>
 *
 *  \copyright
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  \copyright
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

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
