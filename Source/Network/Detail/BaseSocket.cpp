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

#include <Network/Detail/BaseSocket.hpp>

namespace Common
{
namespace Network
{
namespace Detail
{

FileDescriptor::value_type BaseSocket::getNativeHandler() const
{
    return static_cast<FileDescriptor::value_type>(fd);
}

BaseSocket::BaseSocket(FileDescriptor fd)
    : fd(std::move(fd))
{
}

FileDescriptor BaseSocket::connect(const std::string& ip, const std::string port, __socket_type socketType)
{
    FdWithAddrinfo r = socket(ip, port, socketType);

    if (::connect(static_cast<FileDescriptor::value_type>(r.fd), r.address->ai_addr, static_cast<FileDescriptor::value_type>(r.address->ai_addrlen)) == -1)
        throw Exceptions::SystemError(errno);

    return std::move(r.fd);
}

FileDescriptor BaseSocket::bind(const std::string& ip, const std::string port, __socket_type socketType)
{
    FdWithAddrinfo r = socket(ip, port, socketType);

    int yes = 1;
    if (::setsockopt(static_cast<FileDescriptor::value_type>(r.fd), SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        throw Exceptions::SystemError(errno);

    if (::bind(static_cast<FileDescriptor::value_type>(r.fd), r.address->ai_addr, static_cast<FileDescriptor::value_type>(r.address->ai_addrlen)) == -1)
        throw Exceptions::SystemError(errno);

    return std::move(r.fd);
}

FileDescriptor BaseSocket::listen(const std::string& ip, const std::string port)
{
    FileDescriptor fd = bind(ip, port, SOCK_STREAM);

    if (::listen(static_cast<FileDescriptor::value_type>(fd), SOMAXCONN) == -1)
        throw Exceptions::SystemError(errno);

    return fd;
}

void BaseSocket::shutdown()
{
    if (::shutdown(static_cast<FileDescriptor::value_type>(fd), SHUT_RDWR) == -1)
        throw Exceptions::SystemError(errno);
}

BaseSocket::FdWithAddrinfo BaseSocket::socket(const std::string& ip, const std::string port, __socket_type socketType)
{
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;    //! Allows IPv4 or IPv6
    hints.ai_socktype = socketType; //! Basicly TcpIp or Udp
    hints.ai_flags = AI_PASSIVE;    //! Wildcard IP address
    hints.ai_protocol = 0;          //! Any

    addrinfo* address{nullptr};
    if (::getaddrinfo(ip.c_str(), port.c_str(), &hints, &address) != 0)
        throw Exceptions::SystemError(errno);

    std::unique_ptr<addrinfo, Detail::AddrinfoDeleter> _address{address};

    FileDescriptor fd{::socket(address->ai_family, address->ai_socktype, address->ai_protocol)};
    if (!fd)
        throw Exceptions::SystemError(errno);

    FdWithAddrinfo result;
    result.fd = std::move(fd);
    result.address = std::move(_address);
    return result;
}

}
}
}
