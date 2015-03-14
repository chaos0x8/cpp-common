#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

namespace Common
{
namespace Network
{
namespace Detail
{

struct AddrinfoDeleter
{
    void operator () (addrinfo* p)
    {
        freeaddrinfo(p);
    }
};

}
}
}
