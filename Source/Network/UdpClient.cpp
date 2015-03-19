/*!
    \author <https://github.com/chaos0x8>
    \copyright
    Copyright (c) 2015, <https://github.com/chaos0x8>

    \copyright
    Permission to use, copy, modify, and/or distribute this software for any
    purpose with or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    \copyright
    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <Network/UdpClient.hpp>
#include <Network/Detail/BufforSize.hpp>
#include <array>
#include <unistd.h>

namespace Common
{
namespace Network
{

UdpClient::UdpClient(const std::string& ip, const std::string& port)
{
    fd = connect(ip, port, SOCK_DGRAM);
}

void UdpClient::send(const std::string& data)
{
    if (::write(static_cast<int>(fd), data.data(), data.size()) == -1)
        throw Exceptions::SystemError(errno);
}

std::string UdpClient::receive()
{
    std::array<char, BUFFOR_SIZE> buffor;

    int nread = ::read(static_cast<int>(fd), buffor.data(), buffor.size());
    if (nread == -1)
        throw Exceptions::SystemError(errno);
    return std::string(buffor.data(), nread);
}

}
}
