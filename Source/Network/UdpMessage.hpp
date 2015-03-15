#pragma once

#include <Network/UdpHost.hpp>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

namespace Common
{
namespace Network
{

struct UdpMessage
{
    std::string data;
    sockaddr_storage address{};
    socklen_t addressLength{sizeof(sockaddr_storage)};

    //! \throw Exceptions::SocketError
    UdpHost getHost();

    UdpMessage clone(const std::string& data) const;
};

}
}
