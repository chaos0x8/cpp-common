#pragma once

namespace Common
{
namespace Network
{

class FileDescriptor
{
public:
    FileDescriptor() = default;
    explicit FileDescriptor(int fd)
        : fd(std::move(fd))
    {
    }
    FileDescriptor(const FileDescriptor& other) = delete;
    FileDescriptor(FileDescriptor&& other)
        : fd(std::move(other.fd))
    {
        other.fd = INVALID_VALUE;
    }
    ~FileDescriptor()
    {
        close();
    }

    FileDescriptor& operator = (const FileDescriptor&) = delete;
    FileDescriptor& operator = (FileDescriptor&& other)
    {
        this->close();
        this->fd = std::move(other.fd);
        other.fd = INVALID_VALUE;

        return *this;
    }

    explicit operator bool () const
    {
        return fd != INVALID_VALUE;
    }

    explicit operator int () const
    {
        return fd;
    }

    void close()
    {
        if (operator bool())
            ::close(fd);
        fd = INVALID_VALUE;
    }

private:
    static constexpr int INVALID_VALUE = -1;

    int fd{INVALID_VALUE};
};

}
}
