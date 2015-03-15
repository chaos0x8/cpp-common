#include <Network/UdpClient.hpp>
#include <unistd.h>

namespace Common
{
namespace Network
{

UdpClient::UdpClient(const std::string& ip, const std::string& port)
    : buffor(256)
{
    fd = connect(ip, port, SOCK_DGRAM);
}

void UdpClient::send(const std::string& data)
{
    if (::write(static_cast<int>(fd), data.data(), data.size()) == -1)
        throw Exceptions::SocketError(errno);
}

std::string UdpClient::receive()
{
    int nread = ::read(static_cast<int>(fd), buffor.data(), buffor.size());
    if (nread == -1)
        throw Exceptions::SocketError(errno);
    return std::string(buffor.data(), nread);
}

}
}
