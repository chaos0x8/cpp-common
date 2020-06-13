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

#include <Common/RuntimeAssert.hpp>
#include <Network/Detail/BufforSize.hpp>
#include <Network/TcpIpClient.hpp>
#include <array>

namespace Common::Network {
  TcpIpClient::TcpIpClient(const std::string& ip, const std::string& port) {
    fd = connect(ip, port, SOCK_STREAM);
  }

  void TcpIpClient::send(const std::string& data) {
    if (::send(**fd, data.data(), data.size(), 0) == -1) {
      throw Exceptions::SystemError(errno);
    }
  }

  void TcpIpClient::send(const void* src, size_t size) {
    if (::send(**fd, src, size, 0) == -1) {
      throw Exceptions::SystemError(errno);
    }
  }

  std::string TcpIpClient::receive() {
    std::array<char, BUFFOR_SIZE> buffor;

    int r = ::recv(**fd, buffor.data(), buffor.size(), 0);
    if (r == -1) {
      throw Exceptions::SystemError(errno);
    }

    return std::string(buffor.data(), r);
  }

  std::string TcpIpClient::receive(size_t size) {
    std::array<char, BUFFOR_SIZE> buffor;

    Common::runtimeAssert(size <= BUFFOR_SIZE, "size exceeds buffor size");

    int r = ::recv(**fd, buffor.data(), std::min(buffor.size(), size), 0);
    if (r == -1) {
      throw Exceptions::SystemError(errno);
    }

    return std::string(buffor.data(), r);
  }

  bool TcpIpClient::receive(void* dst, size_t size) {
    int r = ::recv(**fd, dst, size, 0);
    if (r == -1) {
      throw Exceptions::SystemError(errno);
    }
    return r > 0 and static_cast<size_t>(r) == size;
  }

  TcpIpClient::TcpIpClient(Detail::FileDescriptor fd)
    : BaseSocket(std::move(fd)) {}
} // namespace Common::Network
