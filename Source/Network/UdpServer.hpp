#pragma once

#include <Network/Detail/BaseSocket.hpp>
#include <Network/UdpMessage.hpp>
#include <vector>

namespace Common
{
namespace Network
{

class UdpServer : public Detail::BaseSocket
{
public:
    //! \throw Exceptions::SocketError
    UdpServer(const std::string& ip, const std::string& port);

    //! \throw Exceptions::SocketError
    UdpMessage receive();

    //! \throw Exceptions::SocketError
    void send(UdpMessage msg);

private:
    std::vector<char> buffor;
    Detail::FileDescriptor fd;
};

}
}
