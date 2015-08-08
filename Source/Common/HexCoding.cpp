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

#include <Common/HexCoding.hpp>
#include <iomanip>
#include <sstream>
#include <cassert>

namespace Common
{
namespace HexCoding
{

std::string encrypt(const std::string& input)
{
    return encrypt(std::vector<char>(std::begin(input), std::end(input)));
}

std::string encrypt(const std::vector<char>& input)
{
    std::stringstream ss;
    ss.setf(std::ios::hex, std::ios::basefield);
    ss.fill('0');
    for (char c : input)
        ss << std::setw(2) << static_cast<uint32_t>(static_cast<uint8_t>(c));
    return ss.str();
}

std::vector<char> decrypt(const std::string& input)
{
    return decrypt(std::vector<char>(std::begin(input), std::end(input)));
}

std::vector<char> decrypt(const std::vector<char>& input)
{
    assert(input.size() % 2 == 0);

    uint32_t value;
    std::stringstream ss;
    std::vector<char> result(input.size() / 2);

    for (size_t i = 0; i < input.size(); i += 2)
    {
        ss.seekg(0);
        ss << std::dec << input[i] << input[i+1];

        ss.seekp(0);
        ss >> std::hex >> value;
        result[i/2] = static_cast<char>(value);
    }

    return result;
}

}
}
