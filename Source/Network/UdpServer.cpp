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

#include <Network/UdpServer.hpp>
#include <Network/Detail/BufforSize.hpp>
#include <array>

namespace Common
{
namespace Network
{

UdpServer::UdpServer(const std::string& ip, const std::string& port)
{
    fd = bind(ip, port, SOCK_DGRAM);
}

UdpMessage UdpServer::receive()
{
    std::array<char, BUFFOR_SIZE> buffor;
    UdpMessage msg{};

    ssize_t nread = ::recvfrom(static_cast<int>(fd), buffor.data(), buffor.size(), 0, reinterpret_cast<sockaddr*>(&msg.address), &msg.addressLength);
    if (nread == -1)
        throw Exceptions::SystemError(errno);

    msg.data = std::string(buffor.data(), nread);

    return msg;
}

void UdpServer::send(UdpMessage msg)
{
    if (::sendto(static_cast<int>(fd), msg.data.data(), msg.data.size(), 0, reinterpret_cast<sockaddr*>(&msg.address), msg.addressLength) != msg.data.size())
        throw Exceptions::SystemError(errno);
}

}
}
