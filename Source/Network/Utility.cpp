/*!
 *  \author <https://github.com/chaos0x8>
 *  \copyright
 *  Copyright (c) 2015 - 2016, <https://github.com/chaos0x8>
 *
 *  \copyright
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  \copyright
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <Network/Utility.hpp>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory>

namespace Common
{
namespace Network
{
namespace Detail
{

class IfAddrsDeleter
{
public:
    void operator ()(ifaddrs* p)
    {
        freeifaddrs(p);
    }
};

std::unique_ptr<ifaddrs, Detail::IfAddrsDeleter> getifaddrs()
{
    ifaddrs* ifAddrStruct{nullptr};
    getifaddrs(&ifAddrStruct);
    return std::unique_ptr<ifaddrs, Detail::IfAddrsDeleter>(ifAddrStruct);
}

}

std::vector<IpAddress> getIpAddresses()
{
    std::vector<IpAddress> result;
    std::unique_ptr<ifaddrs, Detail::IfAddrsDeleter> ifAddrStruct = Detail::getifaddrs();

    for (ifaddrs* ifa = ifAddrStruct.get(); ifa != nullptr; ifa = ifa->ifa_next)
    {
        if (!ifa->ifa_addr)
            continue;

        void* tmpAddrPtr{nullptr};
        switch (ifa->ifa_addr->sa_family)
        {
            case AF_INET:
            {
                tmpAddrPtr = &reinterpret_cast<sockaddr_in*>(ifa->ifa_addr)->sin_addr;
                char addressBuffer[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);

                IpAddress address;
                address.family = AF_INET;
                address.interface = ifa->ifa_name;
                address.address = addressBuffer;
                result.emplace_back(std::move(address));
            }
            break;
            case AF_INET6:
            {
                tmpAddrPtr = &reinterpret_cast<sockaddr_in6*>(ifa->ifa_addr)->sin6_addr;
                char addressBuffer[INET6_ADDRSTRLEN];
                inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);

                IpAddress address;
                address.family = AF_INET6;
                address.interface = ifa->ifa_name;
                address.address = addressBuffer;
                result.emplace_back(std::move(address));
            }
            break;
        }
    }

    return result;
}

}
}
