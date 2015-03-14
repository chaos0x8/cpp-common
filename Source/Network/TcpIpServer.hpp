#pragma once

#include <string>
#include <Network/TcpIpClient.hpp>
#include <Network/FileDescriptor.hpp>
#include <Network/Exceptions/SocketError.hpp>
#include <Network/Detail/AddrinfoDeleter.hpp>
#include <memory>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

namespace Common
{
namespace Network
{

class TcpIpServer
{
public:
    //! \throw Exceptions::SocketError
    TcpIpServer(const std::string& ip, const std::string& port)
    {
        addrinfo hints{};
        hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
        hints.ai_protocol = 0;          /* Any protocol */
        hints.ai_canonname = NULL;
        hints.ai_addr = NULL;
        hints.ai_next = NULL;

        addrinfo* address{nullptr};
        if (::getaddrinfo(ip.c_str(), port.c_str(), &hints, &address) != 0)
            throw Exceptions::SocketError(errno);

        std::unique_ptr<addrinfo, Detail::AddrinfoDeleter> _address{address};

        fd = FileDescriptor{::socket(address->ai_family, address->ai_socktype, address->ai_protocol)};
        if (!fd)
            throw Exceptions::SocketError(errno);

        if (::bind(static_cast<int>(fd), address->ai_addr, static_cast<int>(address->ai_addrlen)) == -1)
            throw Exceptions::SocketError(errno);

        if (::listen(static_cast<int>(fd), SOMAXCONN) == -1)
            throw Exceptions::SocketError(errno);
    }

    //! \throw Exceptions::SocketError
    TcpIpClient accept()
    {
        int acceptResult = ::accept(static_cast<int>(fd), nullptr, nullptr);
        if (acceptResult < 0)
        {
            fd = FileDescriptor{};
            throw Exceptions::SocketError(errno);
        }
        return TcpIpClient{FileDescriptor{acceptResult}};
    }

private:
    FileDescriptor fd;
};

}
}
