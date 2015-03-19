/*!
    \author <https://github.com/chaos0x8>
    \copyright
    Copyright (c) 2015, <https://github.com/chaos0x8>

    \copyright
    Permission to use, copy, modify, and/or distribute this software for any
    purpose with or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    \copyright
    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <Network/Selector.hpp>
#include <Common/Exceptions/SystemError.hpp>

namespace Common
{
namespace Network
{

Selector::Selector()
{
    add(internalCommunication, &Selector::internalHandler, this);
    selectorThread = std::thread(&Selector::threadProcedure, this);
}

Selector::~Selector()
{
    internalCommunication.write("K");
    selectorThread.join();
}

void Selector::add(int fd, SelectorCallback callback)
{
    std::unique_lock<std::mutex> lock(itemsMutex);
    if (items.insert(std::pair<int, SelectorCallback>(fd, std::move(callback))).second)
        internalCommunication.write("U");
}

void Selector::remove(int fd)
{
    std::unique_lock<std::mutex> lock(itemsMutex);
    if (items.erase(fd))
        internalCommunication.write("U");
}

void Selector::threadProcedure()
{
    fd_set fdSet;

    while (keepAlive)
    {
        FD_ZERO(&fdSet);

        int maxFd = 0;
        {
            std::unique_lock<std::mutex> lock(itemsMutex);
            for (const auto& val : items)
            {
                maxFd = std::max(maxFd, val.first);
                FD_SET(val.first, &fdSet);
            }
        }

        if (::select(maxFd + 1, &fdSet, nullptr, nullptr, nullptr) == -1)
            throw Exceptions::SystemError(errno);

        {
            std::unique_lock<std::mutex> lock(itemsMutex);
            for (const auto& val : items)
                if (FD_ISSET(val.first, &fdSet))
                {
                    val.second();
                }
        }
    }
}

void Selector::internalHandler()
{
    if (internalCommunication.read().find("K") != std::string::npos)
        keepAlive = false;
}

}
}
