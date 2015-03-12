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

#include <Common/FileUtility.hpp>
#include <Common/Exceptions/FileLoadError.hpp>
#include <fstream>
#include <vector>

namespace Common
{

bool doesFileExist(const std::string& fileName)
{
    std::ifstream file(fileName, std::ios::in);
    if (!file.is_open())
        return false;
    file.close();
    return true;
}

std::string readFile(const std::string& fileName)
{
    std::ifstream file(fileName.c_str());
    if (!file.is_open())
        throw Exceptions::FileLoadError(fileName);

    file.seekg(0u, std::ios::end);
    size_t length = file.tellg();
    file.seekg(0u, std::ios::beg);

    std::vector<char> bufor(length);
    file.read(bufor.data(), bufor.size());
    file.close();

    return std::string(bufor.data(), bufor.size());
}

}
