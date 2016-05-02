/*!
 *  \author <https://github.com/chaos0x8>
 *  \copyright
 *  Copyright (c) 2016, <https://github.com/chaos0x8>
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

#include <Common/SharedLibrary.hpp>
#include <Common/RuntimeAssert.hpp>
#include <dlfcn.h>

using namespace std::literals;

namespace Common
{

SharedLibrary::SharedLibrary(std::nullptr_t)
    { }

SharedLibrary::SharedLibrary(const std::string& filename)
    { open(filename.c_str(), RTLD_LAZY); }

SharedLibrary::SharedLibrary(const std::string& filename, int flags)
    { open(filename.c_str(), flags); }

SharedLibrary::SharedLibrary(SharedLibrary&& other)
    : handle(std::move(other.handle))
{
    other.handle = nullptr;
}

SharedLibrary::~SharedLibrary()
    { close(); }

SharedLibrary& SharedLibrary::operator = (std::nullptr_t)
{
    close();

    handle = nullptr;

    return *this;
}

SharedLibrary& SharedLibrary::operator = (SharedLibrary&& other)
{
    close();

    handle = std::move(other.handle);
    other.handle = nullptr;

    return *this;
}

SharedLibrary::operator void*()
{
    return handle;
}

SharedLibrary::operator bool() const
{
    return static_cast<bool>(handle);
}

void* SharedLibrary::dlsym(const std::string& symbol)
{
    return dlsym(symbol.c_str());
}

void* SharedLibrary::dlsym(const char* symbol)
{
    return ::dlsym(handle, symbol);
}

void SharedLibrary::open(const char* filename, int flags)
{
    handle = ::dlopen(filename, flags);
    Common::runtimeAssert(static_cast<bool>(handle), "Failed to load dynamic library: "s + filename);
}

void SharedLibrary::close()
{
    if (handle)
        ::dlclose(handle);
}

}
