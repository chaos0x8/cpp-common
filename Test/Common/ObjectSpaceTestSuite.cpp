/*!
 *  \author <https://github.com/chaos0x8>
 *  \copyright
 *  Copyright (c) 2016, <https://github.com/chaos0x8>
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

#include <Common/ObjectSpace.hpp>
#include <string>
#include <gmock/gmock.h>

namespace Common
{
namespace ObjectSpace
{

using namespace testing;

template <class T>
struct Value;

template <>
struct Value<int>
{
    static int v() { return 42; }
};

template <>
struct Value<std::string>
{
    static std::string v() { return "hello"; }
};

template <class T>
struct TestReader : public Test
{
    TestReader()
        : constant([] { return Value<T>::v(); }),
          field([this] { return _field_; })
    { }

    Reader<T> constant;
    Reader<T> field;

    T _field_ = T();
};

using TestTypes = Types<int, std::string>;
TYPED_TEST_CASE(TestReader, TestTypes);

TYPED_TEST(TestReader, retrievesConstant)
{
    using T = TypeParam;
    const auto expected = Value<T>::v();

    EXPECT_THAT(this->constant, Eq(expected));
    EXPECT_THAT(static_cast<T>(this->constant), Eq(expected));
}

TYPED_TEST(TestReader, retrievesField)
{
    using T = TypeParam;

    EXPECT_THAT(this->field, Eq(T()));
}

TYPED_TEST(TestReader, retrievesModifiedField)
{
    using T = TypeParam;
    const auto expected = Value<T>::v();

    this->_field_ = expected;

    EXPECT_THAT(this->field, Eq(expected));
}

template <class T>
struct TestAccessor : public Test
{
    TestAccessor()
        : field1([this] { return _field1_; },
                 [this] (const auto& v) { _field1_ = v; }),
          field2([this] { return _field2_; },
                 [this] (const auto& v) { _field2_ = v; })
    { }

    Accessor<T> field1;
    Accessor<T> field2;

private:
    T _field1_ = T();
    T _field2_ = T();
};

using TestTypes = Types<int, std::string>;
TYPED_TEST_CASE(TestAccessor, TestTypes);

TYPED_TEST(TestAccessor, retrievesField)
{
    using T = TypeParam;

    EXPECT_THAT(this->field1, Eq(T()));
}

TYPED_TEST(TestAccessor, retrievesModifiedField)
{
    using T = TypeParam;
    const auto expected = Value<T>::v();

    this->field1 = expected;

    EXPECT_THAT(this->field1, Eq(expected));
}

TYPED_TEST(TestAccessor, assignsFromAnotherAccessor)
{
    using T = TypeParam;
    const auto expected = Value<T>::v();

    this->field1 = expected;
    EXPECT_TRUE(this->field1 != this->field2);

    this->field2 = this->field1;
    EXPECT_TRUE(this->field1 == this->field2);
}

template <class T>
struct TestWriter : public Test
{
    TestWriter()
        : field([this] (const auto& v) { _field_ = v; }),
          read_field([] { return Value<T>::v(); })
    { }

    Writer<T> field;
    Reader<T> read_field;

    T _field_ = T();
};

using TestTypes = Types<int, std::string>;
TYPED_TEST_CASE(TestWriter, TestTypes);

TYPED_TEST(TestWriter, modifiesField)
{
    const auto expected = Value<TypeParam>::v();

    this->field = expected;

    EXPECT_THAT(this->_field_, Eq(expected));
}

TYPED_TEST(TestWriter, modifiesFieldUsingReader)
{
    const auto expected = Value<TypeParam>::v();

    this->field = this->read_field;

    EXPECT_THAT(this->_field_, Eq(expected));
}

}
}
