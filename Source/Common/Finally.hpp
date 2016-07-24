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

#include <utility>

namespace Common
{
  namespace Detail
  {
    template <typename F>
    struct Finally
    {
      explicit Finally(F&& functor)
        : _valid(true), _functor(std::move(functor)) { }
      Finally(const Finally&) = delete;
      Finally(Finally&& other)
        : _valid(std::move(other._valid)), _functor(std::move(other._functor))
      {
        other._valid = false;
      }

      ~Finally()
      {
        if (_valid)
        {
          try {
            _functor();
          } catch (...) { }
        }
      }

      Finally<F>& operator = (const Finally<F>&) = delete;
      Finally<F>& operator = (Finally<F>&& other)
      {
        this->_valid = std::move(other->_valid);
        this->_functor = std::move(other->_functor);
        other->_valid = false;
        return *this;
      }

      void abort()
      {
        _valid = false;
      }

    private:
      bool _valid;
      F _functor;
    };
  }

  template <typename F>
  inline auto finally(F&& functor)
  {
    return Detail::Finally<F>(std::forward<F>(functor));
  }
}

