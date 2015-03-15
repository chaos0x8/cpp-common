#include <Network/Detail/BaseSocket.hpp>

namespace Common
{
namespace Network
{
namespace Detail
{

FileDescriptor BaseSocket::connect(const std::string& ip, const std::string port, __socket_type socketType)
{
    FdWithAddrinfo r = socket(ip, port, socketType);

    if (::connect(static_cast<int>(r.fd), r.address->ai_addr, static_cast<int>(r.address->ai_addrlen)) == -1)
        throw Exceptions::SocketError(errno);

    return std::move(r.fd);
}

FileDescriptor BaseSocket::bind(const std::string& ip, const std::string port, __socket_type socketType)
{
    FdWithAddrinfo r = socket(ip, port, socketType);

    if (::bind(static_cast<int>(r.fd), r.address->ai_addr, static_cast<int>(r.address->ai_addrlen)) == -1)
        throw Exceptions::SocketError(errno);

    return std::move(r.fd);
}

FileDescriptor BaseSocket::listen(const std::string& ip, const std::string port)
{
    FileDescriptor fd = bind(ip, port, SOCK_STREAM);

    if (::listen(static_cast<int>(fd), SOMAXCONN) == -1)
        throw Exceptions::SocketError(errno);

    return fd;
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
        throw Exceptions::SocketError(errno);

    std::unique_ptr<addrinfo, Detail::AddrinfoDeleter> _address{address};

    FileDescriptor fd{::socket(address->ai_family, address->ai_socktype, address->ai_protocol)};
    if (!fd)
        throw Exceptions::SocketError(errno);

    FdWithAddrinfo result;
    result.fd = std::move(fd);
    result.address = std::move(_address);
    return result;
}

}
}
}

