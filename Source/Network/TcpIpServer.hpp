#pragma once

#include <Network/Detail/BaseSocket.hpp>
#include <Network/TcpIpClient.hpp>

namespace Common
{
namespace Network
{

class TcpIpServer : public Detail::BaseSocket
{
public:
    //! \throw Exceptions::SocketError
    TcpIpServer(const std::string& ip, const std::string& port);

    //! \throw Exceptions::SocketError
    TcpIpClient accept();

private:
    Detail::FileDescriptor fd;
};

}
}
