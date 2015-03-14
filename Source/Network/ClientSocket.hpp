#pragma once

#include <string>
#include <Network/ClientSocket.hpp>
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

class ClientSocket
{
public:
    ClientSocket()
        : buffor(256)
    {
    }

    ClientSocket(FileDescriptor fd)
        : fd(std::move(fd)),
          buffor(256)
    {
    }

    //! \throw Exceptions::SocketError
    ClientSocket(const std::string& ip, const std::string& port)
    {
        addrinfo hints{};
        hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
//        hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
        hints.ai_protocol = 0;          /* Any protocol */
        hints.ai_canonname = NULL;
        hints.ai_addr = NULL;
        hints.ai_next = NULL;

        addrinfo* address{nullptr};
        if (::getaddrinfo(ip.c_str(), port.c_str(), &hints, &address) != 0)
            throw Exceptions::SocketError(errno);

        fd = FileDescriptor{::socket(address->ai_family, address->ai_socktype, address->ai_protocol)};
        if (!fd)
            throw Exceptions::SocketError(errno);

        if (::connect(static_cast<int>(fd), address->ai_addr, static_cast<int>(address->ai_addrlen)) == -1)
            throw Exceptions::SocketError(errno);
    }

    //! \throw Exceptions::SocketError
    void send(const std::string& data)
    {
        if (::send(static_cast<int>(fd), data.data(), data.size(), 0) == -1)
        {
            fd = FileDescriptor{};
            throw Exceptions::SocketError(errno);
        }
    }

    //! \throw Exceptions::SocketError
    std::string receive()
    {
        int r = ::recv(static_cast<int>(fd), buffor.data(), buffor.size(), 0);
        if (r == -1)
        {
            fd = FileDescriptor{};
            throw Exceptions::SocketError(errno);
        }

        if (r == 0)
            fd = FileDescriptor{};

        return std::string(buffor.data(), r);
    }

private:
    FileDescriptor fd;
    std::vector<char> buffor;
};

}
}
