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

#include <Network/UdpMessage.hpp>
#include <Common/Exceptions/SystemError.hpp>
#include <array>

namespace Common
{
namespace Network
{

UdpHost UdpMessage::getHost()
{
    UdpHost udpHost;

    std::array<char, NI_MAXHOST> host;
    std::array<char, NI_MAXSERV> service;

    if (::getnameinfo(reinterpret_cast<sockaddr*>(&address), addressLength, host.data(), host.size(),
        service.data(), service.size(), NI_NUMERICSERV) != 0)
    {
        throw Exceptions::SystemError(errno);
    }

    udpHost.name = host.data();
    udpHost.service = service.data();

    return udpHost;
}

UdpMessage UdpMessage::clone(const std::string& data) const
{
    UdpMessage cloned{};
    cloned.data = data;
    cloned.address = this->address;
    cloned.addressLength = this->addressLength;
    return cloned;
}

}
}
