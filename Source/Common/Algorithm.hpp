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

#include <algorithm>
#include <iterator>
#include <vector>
#include <Common/TypeTraces.hpp>

namespace Common
{
namespace Algorithm
{

template <typename Container> const Container& ref(const Container& c) { return c; };
template <typename Container> const Container& ref(const Container* c) { return *c; }

template <typename Container, typename Functor>
inline bool any(const Container& c, Functor&& f)
{
  return std::any_of(std::cbegin(ref(c)), std::cend(ref(c)), std::forward<Functor>(f));
}

template <
  typename Container,
  typename Item,
  typename std::enable_if< TypeTraces::IsComparable<typename Container::value_type, Item>::value, int>::type = 0
>
inline bool includes(const Container& c, const Item& i)
{
  return std::find(std::cbegin(ref(c)), std::cend(ref(c)), i) != std::cend(ref(c));
}

template <
  typename Container,
  typename Functor,
  typename std::enable_if<!TypeTraces::IsComparable<typename Container::value_type, Functor>::value, int>::type = 0
>
inline bool includes(const Container& c, Functor&& f)
{
  return std::find_if(std::cbegin(ref(c)), std::cend(ref(c)), std::forward<Functor>(f)) != std::cend(ref(c));
}

template <
  template <typename, typename...> class Output = std::vector,
  typename Functor = void,
  typename Container = void
>
inline auto transform(const Container& c, Functor&& f)
{
  auto out = Output<decltype(TypeTraces::ref<Functor>()(TypeTraces::ref<typename Container::value_type>()))>{};
  std::transform(std::cbegin(c), std::cend(c), std::inserter(out, std::end(out)), std::forward<Functor>(f));
  return out;
}

template <
  template <typename, typename...> class Output = std::vector,
  typename Functor = void,
  typename Container = void
>
inline auto filter(const Container& c, Functor&& f)
{
  auto out = Output<typename Container::value_type>{};
  std::copy_if(std::cbegin(c), std::cend(c), std::inserter(out, std::end(out)), std::forward<Functor>(f));
  return out;
}

template <typename Container, typename Functor>
inline auto first(Container& c, Functor&& f)
{
  return std::find_if(std::begin(c), std::end(c), std::forward<Functor>(f));
}

template <
  typename Container,
  typename Item,
  typename std::enable_if< TypeTraces::IsComparable<typename Container::value_type, Item>::value, int>::type = 0
>
inline typename Container::size_type count(const Container& c, const Item& i)
{
  return std::count(std::cbegin(ref(c)), std::cend(ref(c)), i);
}

template <
  typename Container,
  typename Functor,
  typename std::enable_if<!TypeTraces::IsComparable<typename Container::value_type, Functor>::value, int>::type = 0
>
inline typename Container::size_type count(const Container& c, Functor&& f)
{
  return std::count_if(std::cbegin(ref(c)), std::cend(ref(c)), std::forward<Functor>(f));
}

}
}
