#pragma once
#include <string>

namespace Common
{

bool doesFileExist(const std::string& fileName);

std::string readFile(const std::string& fileName);

}
