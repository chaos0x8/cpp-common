#include <Network/TcpIpServer.hpp>

namespace Common
{
namespace Network
{

TcpIpServer::TcpIpServer(const std::string& ip, const std::string& port)
{
    fd = listen(ip, port);
}

TcpIpClient TcpIpServer::accept()
{
    int acceptResult = ::accept(static_cast<int>(fd), nullptr, nullptr);
    if (acceptResult < 0)
    {
        fd = Detail::FileDescriptor{};
        throw Exceptions::SocketError(errno);
    }
    return TcpIpClient{Detail::FileDescriptor{acceptResult}};
}

}
}
