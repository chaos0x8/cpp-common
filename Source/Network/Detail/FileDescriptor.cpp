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

#include <Network/Detail/FileDescriptor.hpp>
#include <unistd.h>
#include <utility>

namespace Common::Network::Detail {
  FileDescriptor::FileDescriptor(value_type fd) : fd(std::move(fd)) {}

  FileDescriptor::FileDescriptor(FileDescriptor&& other)
    : fd(std::move(other.fd)) {
    other.fd = INVALID_VALUE;
  }

  FileDescriptor::~FileDescriptor() {
    close();
  }

  FileDescriptor& FileDescriptor::operator=(FileDescriptor&& other) {
    this->close();
    this->fd = std::move(other.fd);
    other.fd = INVALID_VALUE;

    return *this;
  }

  FileDescriptor::value_type FileDescriptor::operator*() const {
    return fd;
  }

  FileDescriptor::operator bool() const {
    return fd != INVALID_VALUE;
  }

  void FileDescriptor::close() {
    if (operator bool())
      ::close(*fd);
    fd = INVALID_VALUE;
  }

} // namespace Common::Network::Detail
