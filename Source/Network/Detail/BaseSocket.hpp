#pragma once

#include <Network/Exceptions/SocketError.hpp>
#include <Network/Detail/AddrinfoDeleter.hpp>
#include <Network/Detail/FileDescriptor.hpp>
#include <string>
#include <memory>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

namespace Common
{
namespace Network
{
namespace Detail
{

class BaseSocket
{
    struct FdWithAddrinfo
    {
        FileDescriptor fd;
        std::unique_ptr<addrinfo, AddrinfoDeleter> address;
    };

protected:
    //! \throw Exceptions::SocketError
    static FileDescriptor connect(const std::string& ip, const std::string port, __socket_type socketType);

    //! \throw Exceptions::SocketError
    static FileDescriptor bind(const std::string& ip, const std::string port, __socket_type socketType);

    //! \throw Exceptions::SocketError
    static FileDescriptor listen(const std::string& ip, const std::string port);

private:
    //! \throw Exceptions::SocketError
    static FdWithAddrinfo socket(const std::string& ip, const std::string port, __socket_type socketType);
};

}
}
}

