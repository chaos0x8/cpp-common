#pragma once

#include <Network/Detail/BaseSocket.hpp>
#include <vector>

namespace Common
{
namespace Network
{

class TcpIpClient : public Detail::BaseSocket
{
public:
    TcpIpClient();

    //! \throw Exceptions::SocketError
    TcpIpClient(const std::string& ip, const std::string& port);

    //! \throw Exceptions::SocketError
    void send(const std::string& data);

    //! \throw Exceptions::SocketError
    std::string receive();

private:
    TcpIpClient(Detail::FileDescriptor fd);

    std::vector<char> buffor;
    Detail::FileDescriptor fd;

    friend class TcpIpServer;
};

}
}
