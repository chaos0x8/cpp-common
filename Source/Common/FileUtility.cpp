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

size_t fileSize(std::istream& stream)
{
    size_t currentPos = stream.tellg();
    stream.seekg(0u, std::ios::end);
    size_t length = stream.tellg();
    stream.seekg(currentPos, std::ios::beg);
    return length;
}

size_t fileSize(const std::string& fileName)
{
    std::ifstream file(fileName.c_str());
    if (!file.is_open())
        throw Exceptions::FileLoadError(fileName);

    size_t length = fileSize(file);
    file.close();
    return length;
}

std::string readFile(const std::string& fileName)
{
    std::ifstream file(fileName.c_str());
    if (!file.is_open())
        throw Exceptions::FileLoadError(fileName);

    std::vector<char> bufor(fileSize(file));
    file.read(bufor.data(), bufor.size());
    file.close();

    return std::string(bufor.data(), bufor.size());
}

std::string basename(const std::string& fileName)
{
    auto pos = fileName.find_last_of("/");
    if (pos == std::string::npos)
        return fileName;
    return fileName.substr(pos+1, fileName.size());
}

std::string noExtension(const std::string& fileName)
{
    auto pos = fileName.find_last_of("/.");
    if (pos == std::string::npos or fileName[pos] == '/')
        return fileName;
    return fileName.substr(0, pos);
}

}
