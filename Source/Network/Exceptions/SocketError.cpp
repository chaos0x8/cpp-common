#include <Network/Exceptions/SocketError.hpp>
#include <boost/format.hpp>
#include <string.h>

namespace Common
{
namespace Network
{
namespace Exceptions
{

SocketError::SocketError(int errnoValue)
    : std::runtime_error(boost::str(boost::format("Socket error: %1%") % strerror(errnoValue)))
{
}

}
}
}
