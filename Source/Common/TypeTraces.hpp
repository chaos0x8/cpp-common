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

#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>

namespace Common
{
namespace TypeTraces
{
namespace Detail
{
  using YES = uint8_t;
  using NO = uint16_t;
}

template <class T> T& ref();

template <class T, class U>
struct IsComparable
{
  template <class A, class B>
  static Detail::YES check(decltype(TypeTraces::ref<A>() == TypeTraces::ref<B>())*);
  template <class A, class B>
  static Detail::NO check(...);

  enum { value = sizeof(check<T, U>(nullptr)) == sizeof(Detail::YES) };
};

template <class T>
struct IsCArray
{
  template <class U, size_t N>
  static Detail::YES check(const U(&)[N]);
  template <class U>
  static Detail::NO check(const U&);

  enum { value = sizeof(check(ref<T>())) == sizeof(Detail::YES) };
};

template <class T>
struct ValueType
{
  template <class U>
  static auto check(void*) -> typename U::value_type;
  template <class U>
  static auto check(...) -> typename std::decay<decltype(TypeTraces::ref<U>()[0])>::type;

  using type = decltype(check<typename std::decay<T>::type>(nullptr));
};

template <class T>
using ValueType_t = typename ValueType<T>::type;

template <class T>
struct HasDereference
{
  template <class U>
  static Detail::YES check(std::decay_t<decltype(*TypeTraces::ref<std::decay_t<U>>())>*);
  template <class U>
  static Detail::NO check(...);

  enum { value = sizeof(check<T>(nullptr)) == sizeof(Detail::YES) };
};

template <class T>
struct DereferenceType
{
  using type = decltype(*TypeTraces::ref<T>());
};

template <class T>
using DereferenceType_t = typename DereferenceType<T>::type;

}
}
