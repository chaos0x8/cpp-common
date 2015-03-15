#include <Network/Detail/FileDescriptor.hpp>
#include <utility>
#include <unistd.h>

namespace Common
{
namespace Network
{
namespace Detail
{

FileDescriptor::FileDescriptor(int fd)
    : fd(std::move(fd))
{
}

FileDescriptor::FileDescriptor(FileDescriptor&& other)
    : fd(std::move(other.fd))
{
    other.fd = INVALID_VALUE;
}

FileDescriptor::~FileDescriptor()
{
    close();
}

FileDescriptor& FileDescriptor::operator = (FileDescriptor&& other)
{
    this->close();
    this->fd = std::move(other.fd);
    other.fd = INVALID_VALUE;

    return *this;
}

FileDescriptor::operator bool () const
{
    return fd != INVALID_VALUE;
}

FileDescriptor::operator int () const
{
    return fd;
}

void FileDescriptor::close()
{
    if (operator bool())
        ::close(fd);
    fd = INVALID_VALUE;
}

}
}
}
