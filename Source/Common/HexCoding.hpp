#pragma once

#include <string>
#include <vector>

namespace Common
{
namespace HexCoding
{

std::string encrypt(const std::string&);
std::string encrypt(const std::vector<char>&);
std::string decrypt(const std::string&);
std::string decrypt(const std::vector<char>&);

}
}
