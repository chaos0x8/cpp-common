/*!
    \author <https://github.com/chaos0x8>
    \copyright
    Copyright (c) 2015, <https://github.com/chaos0x8>

    \copyright
    Permission to use, copy, modify, and/or distribute this software for any
    purpose with or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    \copyright
    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#pragma once

#include <memory>
#include <gmock/gmock.h>

namespace Common
{
namespace Detail
{

template <class T> struct UnPtr;

template <class T> struct UnPtr<std::unique_ptr<T>>
{
    using type = T;
    using matcher_type = const std::unique_ptr<T>&;
};

template <class T> struct UnPtr<std::shared_ptr<T>>
{
    using type = T;
    using matcher_type = std::shared_ptr<T>;
};

template <class T, class U>
class PtrFieldMatcher : public testing::MatcherInterface<typename UnPtr<U>::matcher_type>
{
public:
    using C = typename UnPtr<U>::type;

    explicit PtrFieldMatcher(T C::*field, const typename std::decay<T>::type& expected)
        : field(field), expected(expected) { }

    bool MatchAndExplain(typename UnPtr<U>::matcher_type object, testing::MatchResultListener* listener) const override
    {
        return (*object).*field == expected;
    }

    void DescribeTo(std::ostream* os) const override
    {
        *os << "field equals " << expected;
    }

    void DescribeNegationTo(std::ostream* os) const override
    {
        *os << "field does not equal " << expected;
    }

private:
    T C::*field;
    typename std::decay<T>::type expected;
};

template <class T, class U>
class PtrPropertyMatcher : public testing::MatcherInterface<typename UnPtr<U>::matcher_type>
{
public:
    using C = typename UnPtr<U>::type;

    explicit PtrPropertyMatcher(T (C::*property)() const, const typename std::decay<T>::type& expected)
        : property(property), expected(expected) { }

    bool MatchAndExplain(typename UnPtr<U>::matcher_type object, testing::MatchResultListener* listener) const override
    {
        return ((*object).*property)() == expected;
    }

    void DescribeTo(std::ostream* os) const override
    {
        *os << "property equals " << expected;
    }

    void DescribeNegationTo(std::ostream* os) const override
    {
        *os << "property does not equal " << expected;
    }

private:
    T (C::*property)() const;
    typename std::decay<T>::type expected;
};

}

template <template <class, class...> class Ptr, class U, class T, class K>
inline testing::Matcher<typename Detail::UnPtr<Ptr<U>>::matcher_type> PtrField(T U::*field, const K& expected)
{
    return MakeMatcher(new Detail::PtrFieldMatcher<T, Ptr<U>>(field, expected));
}

template <template <class, class...> class Ptr, class U, class T, class K>
inline testing::Matcher<typename Detail::UnPtr<Ptr<U>>::matcher_type> PtrProperty(T (U::*property)() const, const K& expected)
{
    return MakeMatcher(new Detail::PtrPropertyMatcher<T, Ptr<U>>(property, expected));
}

}
