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

#pragma once

#include <Network/Detail/BaseSocket.hpp>

namespace Common
{
namespace Network
{

class UdpClient : public Detail::BaseSocket
{
public:
    UdpClient() noexcept = default;

    //! \throw Exceptions::SystemError
    UdpClient(const std::string& ip, const std::string& port);

    //! \throw Exceptions::SystemError
    void send(const std::string& data);

    //! \throw Exceptions::SystemError
    std::string receive();

    //! \throw Exceptions::SystemError
    using Detail::BaseSocket::shutdown;
};

}
}
