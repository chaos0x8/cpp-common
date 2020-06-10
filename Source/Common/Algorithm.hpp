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

#include <Common/Exceptions/NoResult.hpp>
#include <Common/TypeTraces.hpp>
#include <algorithm>
#include <iterator>
#include <optional>
#include <vector>

namespace Common {
  namespace Algorithm {
    template <typename Container> Container& ref(Container& c) {
      return c;
    }
    template <typename Container> Container& ref(Container* c) {
      return *c;
    }

    // ------------------------------------------------------------------------------

    template <class Container, class Functor>
    inline bool all(const Container& c, Functor&& f) {
      return std::all_of(std::begin(c), std::end(c), std::forward<Functor>(f));
    }

    template <typename Container, typename Functor>
    inline bool any(const Container& c, Functor&& f) {
      return std::any_of(std::begin(c), std::end(c), std::forward<Functor>(f));
    }

    template <typename Container, typename Item>
    inline bool includes(Container&& c, Item&& i) {
      if constexpr (TypeTraces::IsComparable_v<
                      TypeTraces::ValueType_t<Container>, Item>) {
        return std::find(std::begin(c), std::end(c), std::forward<Item>(i)) !=
               std::end(c);
      } else {
        return std::find_if(std::begin(c), std::end(c),
                 std::forward<Item>(i)) != std::end(c);
      }
    }

    template <template <typename, typename...> class Output = std::vector,
      typename Functor = void, typename Container = void>
    inline auto transform(Container&& c, Functor&& f) {
      auto out = Output<decltype(TypeTraces::ref<Functor>()(
        TypeTraces::ref<TypeTraces::ValueType_t<Container>>()))>{};
      std::transform(std::begin(c), std::end(c),
        std::inserter(out, std::end(out)), std::forward<Functor>(f));
      return out;
    }

    template <template <typename, typename...> class Output = std::vector,
      typename Functor = void, typename Container = void>
    inline auto filter(Container&& c, Functor&& f) {
      auto out = Output<TypeTraces::ValueType_t<Container>>{};
      std::copy_if(std::begin(c), std::end(c),
        std::inserter(out, std::end(out)), std::forward<Functor>(f));
      return out;
    }

    template <typename Container, typename Item>
    inline auto first(Container&& c, Item&& i)
      -> std::optional<TypeTraces::ValueType_t<Container>> {
      if constexpr (TypeTraces::IsComparable_v<
                      TypeTraces::ValueType_t<Container>, Item>) {
        auto it = std::find(std::begin(c), std::end(c), std::forward<Item>(i));
        if (it == std::end(c)) {
          return std::nullopt;
        }
        return *it;
      } else {
        auto it =
          std::find_if(std::begin(c), std::end(c), std::forward<Item>(i));
        if (it == std::end(c)) {
          return std::nullopt;
        }
        return *it;
      }
    }

    template <typename Container, typename Item>
    inline auto count(const Container& c, Item&& i) {
      if constexpr (TypeTraces::IsComparable_v<
                      TypeTraces::ValueType_t<Container>, Item>) {
        return std::count(std::begin(c), std::end(c), std::forward<Item>(i));
      } else {
        return std::count_if(std::begin(c), std::end(c), std::forward<Item>(i));
      }
    }

    template <typename Container> inline void sort(Container&& c) {
      std::sort(std::begin(c), std::end(c));
    }

    template <typename Container, typename Comparator>
    inline void sort(Container&& c, Comparator&& comp) {
      std::sort(std::begin(c), std::end(c), std::forward<Comparator>(comp));
    }

    template <typename Container>
    inline auto max(Container&& c)
      -> std::optional<TypeTraces::ValueType_t<Container>> {
      auto it = std::max_element(std::begin(c), std::end(c));
      if (it == std::end(c)) {
        return std::nullopt;
      }
      return *it;
    }

    template <typename Container, typename Comparator>
    inline auto max(Container&& c, Comparator&& comp)
      -> std::optional<TypeTraces::ValueType_t<Container>> {
      auto it = std::max_element(
        std::begin(c), std::end(c), std::forward<Comparator>(comp));
      if (it == std::end(c)) {
        return std::nullopt;
      }
      return *it;
    }

    template <typename Container, typename Item>
    inline void erase(Container&& c, Item&& i) {
      if constexpr (TypeTraces::IsComparable_v<
                      TypeTraces::ValueType_t<Container>, Item>) {
        Algorithm::ref(c).erase(
          std::remove(std::begin(c), std::end(c), std::forward<Item>(i)),
          std::end(c));
      } else {
        Algorithm::ref(c).erase(
          std::remove_if(std::begin(c), std::end(c), std::forward<Item>(i)),
          std::end(c));
      }
    }

    template <typename Container, typename R>
    inline void forEach(Container&& c,
      R (std::decay_t<TypeTraces::ValueType_t<Container>>::*f)()) {
      for (auto& i : c)
        (Algorithm::ref(i).*f)();
    }

    template <typename Container, typename R,
      typename std::enable_if<
        TypeTraces::HasDereference<TypeTraces::ValueType_t<Container>>::value,
        int>::type = 0>
    inline void forEach(
      Container&& c, R (std::decay<TypeTraces::DereferenceType_t<
                         TypeTraces::ValueType_t<Container>>>::type::*f)()) {
      for (auto& i : c)
        (Algorithm::ref(*i).*f)();
    }

    template <typename Container, typename Functor>
    inline void forEach(Container&& c, Functor&& f) {
      for (auto& i : c)
        f(i);
    }

  } // namespace Algorithm
} // namespace Common
