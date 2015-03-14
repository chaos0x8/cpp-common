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

struct UdpHost
{
    std::string name;
    std::string service;
};

struct UdpMessage
{
    std::string data;
    sockaddr_storage address{};
    socklen_t addressLength{sizeof(sockaddr_storage)};

    //! \throw Exceptions::SocketError
    UdpHost getHost()
    {
        UdpHost udpHost;

        std::array<char, NI_MAXHOST> host;
        std::array<char, NI_MAXSERV> service;

        if (::getnameinfo(reinterpret_cast<sockaddr*>(&address), addressLength, host.data(), host.size(),
            service.data(), service.size(), NI_NUMERICSERV) != 0)
        {
            throw Exceptions::SocketError(errno);
        }

        udpHost.name = host.data();
        udpHost.service = service.data();

        return udpHost;
    }

    UdpMessage clone(const std::string& data)
    {
        UdpMessage cloned{};
        cloned.data = data;
        cloned.address = this->address;
        cloned.addressLength = this->addressLength;
        return cloned;
    }
};

class UdpServer
{
public:
    //! \throw Exceptions::SocketError
    UdpServer(const std::string& ip, const std::string& port)
        : buffor(256)
    {
        addrinfo hints{};
        hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
        hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
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
    }

    //! \throw Exceptions::SocketError
    UdpMessage receive()
    {
        UdpMessage msg{};

        ssize_t nread = ::recvfrom(static_cast<int>(fd), buffor.data(), buffor.size(), 0, reinterpret_cast<sockaddr*>(&msg.address), &msg.addressLength);
        if (nread == -1)
            throw Exceptions::SocketError(errno);

        msg.data = std::string(buffor.data(), nread);

        return msg;
    }

    //! \throw Exceptions::SocketError
    void send(UdpMessage msg)
    {
        if (::sendto(static_cast<int>(fd), msg.data.data(), msg.data.size(), 0, reinterpret_cast<sockaddr*>(&msg.address), msg.addressLength) != msg.data.size())
            throw Exceptions::SocketError(errno);
    }

private:
    std::vector<char> buffor;
    FileDescriptor fd;
};

}
}
