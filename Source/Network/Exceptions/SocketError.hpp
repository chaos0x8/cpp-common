#pragma once

#include <stdexcept>

namespace Common
{
namespace Network
{
namespace Exceptions
{

class SocketError : public std::runtime_error
{
public:
    explicit SocketError(int errnoValue);
};

}
}
}
