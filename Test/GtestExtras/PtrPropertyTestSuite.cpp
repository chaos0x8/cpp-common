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

#include <GtestExtras/PtrProperty.hpp>
#include <GtestExtras/MockFunction.hpp>
#include <gmock/gmock.h>

namespace Common
{
namespace UT
{

using namespace testing;

struct Foo
{
    std::string property() const { return _property; }

    void property(const std::string& value) { _property = value; }

    uint32_t field = 0;

private:
    std::string _property;
};

class PtrPropertyTestSuite : public Test
{
public:
    Foo* fill(Foo* foo) const
    {
        foo->field = FIELD;
        foo->property(PROPERTY);
        return foo;
    }

    template <template <class, class...> class Ptr>
    void testMatchesCorrectValue()
    {
        auto foo = Ptr<Foo>(fill(new Foo));

        ASSERT_THAT(foo, PtrField<Ptr>(&Foo::field, FIELD));
        ASSERT_THAT(foo, PtrProperty<Ptr>(&Foo::property, PROPERTY));
    }

    template <template <class, class...> class Ptr>
    void testFailsWhenValueIsIncorrect()
    {
        auto foo = Ptr<Foo>(new Foo);

        ASSERT_THAT(foo, Not(PtrField<Ptr>(&Foo::field, FIELD)));
        ASSERT_THAT(foo, Not(PtrProperty<Ptr>(&Foo::property, PROPERTY)));
    }

    template <template <class, class...> class Ptr>
    void testMatchesValueInExpectCall()
    {
        auto foo = Ptr<Foo>(fill(new Foo));

        StrictMock<MockFunction<void (const Ptr<Foo>&)>> mock;

        EXPECT_CALL(mock, Call(AllOf(
            PtrField<Ptr>(&Foo::field, FIELD), PtrProperty<Ptr>(&Foo::property, PROPERTY))));

        mock.Call(foo);
    }

    template <template <class, class...> class Ptr>
    void testMatchesValueInExpectCallViaCopy()
    {
        auto foo = Ptr<Foo>(fill(new Foo));

        StrictMock<MockFunction<void (Ptr<Foo>)>> mock;

        EXPECT_CALL(mock, Call(AllOf(
            PtrField<Ptr>(&Foo::field, FIELD), PtrProperty<Ptr>(&Foo::property, PROPERTY))));

        mock.Call(foo);
    }

    const uint32_t FIELD = 42;
    const std::string PROPERTY = "hello";
};

TEST_F(PtrPropertyTestSuite, matchesCorrectValue)
{
    testMatchesCorrectValue<std::unique_ptr>();
    testMatchesCorrectValue<std::shared_ptr>();
}

TEST_F(PtrPropertyTestSuite, failsWhenValueIsIncorrect)
{
    testFailsWhenValueIsIncorrect<std::unique_ptr>();
    testFailsWhenValueIsIncorrect<std::shared_ptr>();
}

TEST_F(PtrPropertyTestSuite, matchesValueInExpectCall)
{
    testMatchesValueInExpectCall<std::unique_ptr>();
    testMatchesValueInExpectCall<std::shared_ptr>();
}

TEST_F(PtrPropertyTestSuite, matchesValueInExpectCallViaCopy)
{
    testMatchesValueInExpectCallViaCopy<std::shared_ptr>();
}

}
}
