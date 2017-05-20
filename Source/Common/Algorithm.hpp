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
#include <Common/Exceptions/NoResult.hpp>

namespace Common
{
namespace Algorithm
{

// ------------------------------------------------------------------------------
template <typename Container>
auto cbegin(const Container& c) { return std::cbegin(c); }

template <
  typename Container,
  typename std::enable_if<!TypeTraces::IsCArray<Container>::value, int>::type = 0
>
auto cbegin(const Container* c) { return std::cbegin(*c); }

template <typename Container>
auto cend(const Container& c) { return std::cend(c); }

template <
  typename Container,
  typename std::enable_if<!TypeTraces::IsCArray<Container>::value, int>::type = 0
>
auto cend(const Container* c) { return std::cend(*c); }
// ------------------------------------------------------------------------------
template <typename Container>
auto begin(Container& c) { return std::begin(c); }

template <
  typename Container,
  typename std::enable_if<!TypeTraces::IsCArray<Container>::value, int>::type = 0
>
auto begin(Container* c) { return std::begin(*c); }

template <typename Container>
auto end(Container& c) { return std::end(c); }

template <
  typename Container,
  typename std::enable_if<!TypeTraces::IsCArray<Container>::value, int>::type = 0
>
auto end(Container* c) { return std::end(*c); }
// ------------------------------------------------------------------------------
template <typename Container> Container& ref(Container& c) { return c; }
template <typename Container> Container& ref(Container* c) { return *c; }
// ------------------------------------------------------------------------------

template <typename Container, typename Functor>
inline bool all(Container&& c, Functor&& f)
{
  return std::all_of(Algorithm::cbegin<Container>(c), Algorithm::cend<Container>(c), std::forward<Functor>(f));
}

template <typename Container, typename Functor>
inline bool any(const Container& c, Functor&& f)
{
  return std::any_of(Algorithm::cbegin<Container>(c), Algorithm::cend<Container>(c), std::forward<Functor>(f));
}

template <
  typename Container,
  typename Item,
  typename std::enable_if< TypeTraces::IsComparable<TypeTraces::ValueType_t<Container>, Item>::value, int>::type = 0
>
inline bool includes(Container&& c, const Item& i)
{
  return std::find(Algorithm::cbegin<Container>(c), Algorithm::cend<Container>(c), i) != Algorithm::cend<Container>(c);
}

template <
  typename Container,
  typename Functor,
  typename std::enable_if<!TypeTraces::IsComparable<TypeTraces::ValueType_t<Container>, Functor>::value, int>::type = 0
>
inline bool includes(Container&& c, Functor&& f)
{
  return std::find_if(Algorithm::cbegin<Container>(c), Algorithm::cend<Container>(c), std::forward<Functor>(f)) != Algorithm::cend<Container>(c);
}

template <
  template <typename, typename...> class Output = std::vector,
  typename Functor = void,
  typename Container = void
>
inline auto transform(Container&& c, Functor&& f)
{
  auto out = Output<decltype(TypeTraces::ref<Functor>()(TypeTraces::ref<TypeTraces::ValueType_t<Container>>()))>{};
  std::transform(Algorithm::cbegin<Container>(c), Algorithm::cend<Container>(c), std::inserter(out, std::end(out)), std::forward<Functor>(f));
  return out;
}

template <
  template <typename, typename...> class Output = std::vector,
  typename Functor = void,
  typename Container = void
>
inline auto filter(Container&& c, Functor&& f)
{
  auto out = Output<TypeTraces::ValueType_t<Container>>{};
  std::copy_if(Algorithm::cbegin<Container>(c), Algorithm::cend<Container>(c), std::inserter(out, std::end(out)), std::forward<Functor>(f));
  return out;
}

template <
  typename Container,
  typename Item,
  typename std::enable_if< TypeTraces::IsComparable<TypeTraces::ValueType_t<Container>, Item>::value, int>::type = 0
>
inline auto first(Container&& c, const Item& i)
{
  return std::find(Algorithm::begin<Container>(c), Algorithm::end<Container>(c), i);
}

template <
  typename Container,
  typename Functor,
  typename std::enable_if<!TypeTraces::IsComparable<TypeTraces::ValueType_t<Container>, Functor>::value, int>::type = 0
>
inline auto first(Container&& c, Functor&& f)
{
  return std::find_if(Algorithm::begin<Container>(c), Algorithm::end<Container>(c), std::forward<Functor>(f));
}

template <
  typename Container,
  typename Item,
  typename std::enable_if< TypeTraces::IsComparable<TypeTraces::ValueType_t<Container>, Item>::value, int>::type = 0
>
inline auto count(const Container& c, const Item& i)
{
  return std::count(Algorithm::cbegin<Container>(c), Algorithm::cend<Container>(c), i);
}

template <
  typename Container,
  typename Functor,
  typename std::enable_if<!TypeTraces::IsComparable<TypeTraces::ValueType_t<Container>, Functor>::value, int>::type = 0
>
inline auto count(Container&& c, Functor&& f)
{
  return std::count_if(Algorithm::cbegin<Container>(c), Algorithm::cend<Container>(c), std::forward<Functor>(f));
}

template <
  typename Container
>
inline void sort(Container&& c)
{
  std::sort(Algorithm::begin<Container>(c), Algorithm::end<Container>(c));
}

template <
  typename Container,
  typename Comparator
>
inline void sort(Container&& c, Comparator&& comp)
{
  std::sort(Algorithm::begin<Container>(c), Algorithm::end<Container>(c), std::forward<Comparator>(comp));
}

template <
  typename Container
>
inline const auto& max(Container&& c)
{
  auto it = std::max_element(Algorithm::cbegin<Container>(c), Algorithm::cend<Container>(c));
  if (it == Algorithm::cend<Container>(c)) throw Exceptions::NoResult();
  return *it;
}

template <
  typename Container,
  typename Comparator
>
inline const auto& max(Container&& c, Comparator&& comp)
{
  auto it = std::max_element(Algorithm::cbegin<Container>(c), Algorithm::cend<Container>(c), std::forward<Comparator>(comp));
  if (it == Algorithm::cend<Container>(c)) throw Exceptions::NoResult();
  return *it;
}

template <
  typename Container,
  typename Item,
  typename std::enable_if< TypeTraces::IsComparable<TypeTraces::ValueType_t<Container>, Item>::value, int>::type = 0
>
inline void erase(Container&& c, const Item& i)
{
  Algorithm::ref(c).erase(
    std::remove(Algorithm::begin<Container>(c), Algorithm::end<Container>(c), i),
    Algorithm::end<Container>(c));
}

template <
  typename Container,
  typename Functor,
  typename std::enable_if<!TypeTraces::IsComparable<TypeTraces::ValueType_t<Container>, Functor>::value, int>::type = 0
>
inline void erase(Container&& c, Functor&& f)
{
  Algorithm::ref(c).erase(
    std::remove_if(Algorithm::begin<Container>(c), Algorithm::end<Container>(c), std::forward<Functor>(f)),
    Algorithm::end<Container>(c));
}

template <
  typename Container,
  typename R
>
inline void forEach(Container&& c, R (std::decay_t<TypeTraces::ValueType_t<Container>>::* f)())
{
  for (auto& i : c) (Algorithm::ref(i).*f)();
}

template <
  typename Container,
  typename R,
  typename std::enable_if<TypeTraces::HasDereference<TypeTraces::ValueType_t<Container>>::value, int>::type = 0
>
inline void forEach(Container&& c, R (std::decay<TypeTraces::DereferenceType_t<TypeTraces::ValueType_t<Container>>>::type::* f)())
{
  for (auto& i : c) (Algorithm::ref(*i).*f)();
}

template <
  typename Container,
  typename Functor
>
inline void forEach(Container&& c, Functor&& f)
{
  for (auto& i : c) f(i);
}

}
}
