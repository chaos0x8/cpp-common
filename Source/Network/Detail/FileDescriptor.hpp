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

#pragma once

#include "Common/StrongType.hpp"
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

namespace Common::Network {
  namespace Detail {

    class FileDescriptor {
    public:
      using value_type = StrongType<int, struct FileDescriptorTag>;

      FileDescriptor() = default;
      explicit FileDescriptor(value_type fd);
      FileDescriptor(const FileDescriptor& other) = delete;
      FileDescriptor(FileDescriptor&& other);
      ~FileDescriptor();

      FileDescriptor& operator=(const FileDescriptor&) = delete;
      FileDescriptor& operator=(FileDescriptor&& other);

      value_type operator*() const;
      explicit operator bool() const;

      void close();

    private:
      static constexpr value_type INVALID_VALUE = value_type(-1);

      value_type fd{INVALID_VALUE};
    };
  } // namespace Detail

  using NativeHandler = Detail::FileDescriptor::value_type;
} // namespace Common::Network
