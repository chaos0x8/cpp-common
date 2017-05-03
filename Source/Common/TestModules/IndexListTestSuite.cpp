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
    static void ignore(Args...)
    {

    }

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
        ignore( f1(std::get<T>(values))... );
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
