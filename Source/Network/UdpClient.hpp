#pragma once

#include <Network/Detail/BaseSocket.hpp>
#include <vector>

namespace Common
{
namespace Network
{

class UdpClient : public Detail::BaseSocket
{
public:
    //! \throw Exceptions::SocketError
    UdpClient(const std::string& ip, const std::string& port);

    //! \throw Exceptions::SocketError
    void send(const std::string& data);

    //! \throw Exceptions::SocketError
    std::string receive();

private:
    std::vector<char> buffor;
    Detail::FileDescriptor fd;
};

}
}
