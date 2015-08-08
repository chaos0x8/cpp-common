#pragma once

#include <string>
#include <vector>

namespace Common
{
namespace HexCoding
{

std::string encode(const std::string&);
std::string encode(const std::vector<char>&);
std::string decode(const std::string&);
std::string decode(const std::vector<char>&);

}
}
