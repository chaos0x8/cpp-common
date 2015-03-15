#include <Network/UdpServer.hpp>

namespace Common
{
namespace Network
{

UdpServer::UdpServer(const std::string& ip, const std::string& port)
    : buffor(256)
{
    fd = bind(ip, port, SOCK_DGRAM);
}

UdpMessage UdpServer::receive()
{
    UdpMessage msg{};

    ssize_t nread = ::recvfrom(static_cast<int>(fd), buffor.data(), buffor.size(), 0, reinterpret_cast<sockaddr*>(&msg.address), &msg.addressLength);
    if (nread == -1)
        throw Exceptions::SocketError(errno);

    msg.data = std::string(buffor.data(), nread);

    return msg;
}

void UdpServer::send(UdpMessage msg)
{
    if (::sendto(static_cast<int>(fd), msg.data.data(), msg.data.size(), 0, reinterpret_cast<sockaddr*>(&msg.address), msg.addressLength) != msg.data.size())
        throw Exceptions::SocketError(errno);
}

}
}
