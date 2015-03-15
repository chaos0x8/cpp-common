#include <Network/TcpIpClient.hpp>

namespace Common
{
namespace Network
{

TcpIpClient::TcpIpClient()
    : buffor(256)
{
}

TcpIpClient::TcpIpClient(const std::string& ip, const std::string& port)
{
    fd = connect(ip, port, SOCK_STREAM);
}

void TcpIpClient::send(const std::string& data)
{
    if (::send(static_cast<int>(fd), data.data(), data.size(), 0) == -1)
    {
        fd = Detail::FileDescriptor{};
        throw Exceptions::SocketError(errno);
    }
}

std::string TcpIpClient::receive()
{
    int r = ::recv(static_cast<int>(fd), buffor.data(), buffor.size(), 0);
    if (r == -1)
    {
        fd = Detail::FileDescriptor{};
        throw Exceptions::SocketError(errno);
    }

    if (r == 0)
        fd = Detail::FileDescriptor{};

    return std::string(buffor.data(), r);
}

TcpIpClient::TcpIpClient(Detail::FileDescriptor fd)
    : buffor(256),
      fd(std::move(fd))
{
}

}
}
