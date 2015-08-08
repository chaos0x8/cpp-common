#include <Common/HexCoding.hpp>
#include <iomanip>
#include <sstream>
#include <cassert>

namespace Common
{
namespace HexCoding
{

std::string encode(const std::string& input)
{
    std::stringstream ss;
    ss.setf(std::ios::hex, std::ios::basefield);
    ss.fill('0');
    for (char c : input)
        ss << std::setw(2) << static_cast<uint32_t>(static_cast<uint8_t>(c));
    return ss.str();
}

std::string encode(const std::vector<char>& input)
{
    return encode(std::string(input.data(), input.size()));

}

std::string decode(const std::string& input)
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

    return std::string(result.data(), result.size());
}

std::string decode(const std::vector<char>& input)
{
    return decode(std::string(input.data(), input.size()));
}

}
}
