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

class FileDescriptor
{
public:
    FileDescriptor() = default;
    explicit FileDescriptor(int fd);
    FileDescriptor(const FileDescriptor& other) = delete;
    FileDescriptor(FileDescriptor&& other);
    ~FileDescriptor();

    FileDescriptor& operator = (const FileDescriptor&) = delete;
    FileDescriptor& operator = (FileDescriptor&& other);

    explicit operator bool () const;
    explicit operator int () const;

    void close();

private:
    static constexpr int INVALID_VALUE = -1;

    int fd{INVALID_VALUE};
};

}
}
}
