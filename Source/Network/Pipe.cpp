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

#include <Network/Pipe.hpp>
#include <Common/Exceptions/SystemError.hpp>
#include <Network/Detail/BufforSize.hpp>
#include <array>
#include <unistd.h>

namespace Common
{
namespace Network
{

Pipe::Pipe()
{
    int fds[2] = {};
    if (::pipe(fds) == -1)
        throw Exceptions::SystemError(errno);
    readPipe = Detail::FileDescriptor{fds[0]};
    writePipe = Detail::FileDescriptor{fds[1]};
}

std::string Pipe::read()
{
    std::array<char, BUFFOR_SIZE> buffor;
    ssize_t nread = ::read(static_cast<int>(readPipe), buffor.data(), buffor.size());
    if (nread == -1)
        throw Exceptions::SystemError(errno);
    return std::string(buffor.data(), nread);
}

void Pipe::write(const std::string& buffor)
{
    if (::write(static_cast<int>(writePipe), buffor.data(), buffor.size()) == -1)
        throw Exceptions::SystemError(errno);
}

std::array<int, 2> Pipe::getNativeHandler() const
{
    return { static_cast<int>(readPipe), static_cast<int>(writePipe) };
}

}
}
