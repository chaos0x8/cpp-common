/*!
 *  \author <https://github.com/chaos0x8>
 *  \copyright
 *  Copyright (c) 2016 - 2017, <https://github.com/chaos0x8>
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

#pragma once

#include <cstddef>
#include <string>

namespace Common
{

class SharedLibrary
{
public:
    SharedLibrary(std::nullptr_t);
    SharedLibrary(const std::string& filename);
    SharedLibrary(const std::string& filename, int flags);
    SharedLibrary(const SharedLibrary&) = delete;
    SharedLibrary(SharedLibrary&&);
    ~SharedLibrary();

    SharedLibrary& operator = (const SharedLibrary&) = delete;
    SharedLibrary& operator = (std::nullptr_t);
    SharedLibrary& operator = (SharedLibrary&&);

    explicit operator void*();
    explicit operator bool() const;

    void* dlsym(const std::string&);
    void* dlsym(const char*);

private:
    void open(const char* filename, int flags);
    void close();

    void* handle = nullptr;
};

}
