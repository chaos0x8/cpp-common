/*!
 *  \author <https://github.com/chaos0x8>
 *  \copyright
 *  Copyright (c) 2017, <https://github.com/chaos0x8>
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

#include "NamedArgs.hpp"
#include "Exceptions.hpp"
#include "Detail/MatchName.hpp"
#include <algorithm>

namespace Common::OptionParser
{
  NamedArgs::NamedArgs(std::vector<Item> items)
    : _items(std::move(items))
  {
  }

  NamedArgs::~NamedArgs() = default;

  std::string NamedArgs::operator[](std::string name) const
  {
    auto it = std::find_if(std::begin(_items), std::end(_items), [&](const auto& item) {
      return Detail::matchName(item.keys, name);
    });

    if (it == std::end(_items))
      throw UnknownOptionError(name);

    return it->value;
  }
}
