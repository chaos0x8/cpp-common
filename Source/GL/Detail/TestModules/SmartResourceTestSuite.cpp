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

#include <GL/Detail/SmartResource.hpp>
#include <memory>
#include <gmock/gmock.h>

namespace Common
{
namespace GL
{
namespace Detail
{
namespace Ut
{

struct FakeDeleter
{
    void operator() (int value) const
    {
        calls.push_back(value);
    }

    static std::vector<int> calls;
};
std::vector<int> FakeDeleter::calls;

struct FakeAllocator
{
    int operator() () const
    {
        return ++resultValue;
    }

    static int resultValue;
};
int FakeAllocator::resultValue = 0;

using namespace testing;
using Sut = SmartResource<int, FakeDeleter, FakeAllocator>;

class SmartResourceTestSuite : public Test
{
public:
    SmartResourceTestSuite()
        : sut(new Sut)
    {
    }
    ~SmartResourceTestSuite()
    {
        FakeDeleter::calls.clear();
        FakeAllocator::resultValue = 0;
    }

    std::unique_ptr<Sut> sut;
};

TEST_F(SmartResourceTestSuite, shouldBeEmpty)
{
    ASSERT_THAT(*sut, Eq(int()));
}

TEST_F(SmartResourceTestSuite, shouldAllocateAndDeallocate)
{
    *sut = Sut::allocate();
    ASSERT_THAT(*sut, Eq(1));

    sut = nullptr;

    ASSERT_THAT(FakeDeleter::calls, ElementsAre(1));
}

}
}
}
}
