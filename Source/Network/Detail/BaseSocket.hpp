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

#include <Network/Exceptions/SocketError.hpp>
#include <Network/Detail/AddrinfoDeleter.hpp>
#include <Network/Detail/FileDescriptor.hpp>
#include <string>
#include <memory>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

namespace Common
{
namespace Network
{
namespace Detail
{

class BaseSocket
{
    struct FdWithAddrinfo
    {
        FileDescriptor fd;
        std::unique_ptr<addrinfo, AddrinfoDeleter> address;
    };

protected:
    //! \throw Exceptions::SocketError
    static FileDescriptor connect(const std::string& ip, const std::string port, __socket_type socketType);

    //! \throw Exceptions::SocketError
    static FileDescriptor bind(const std::string& ip, const std::string port, __socket_type socketType);

    //! \throw Exceptions::SocketError
    static FileDescriptor listen(const std::string& ip, const std::string port);

private:
    //! \throw Exceptions::SocketError
    static FdWithAddrinfo socket(const std::string& ip, const std::string port, __socket_type socketType);
};

}
}
}
