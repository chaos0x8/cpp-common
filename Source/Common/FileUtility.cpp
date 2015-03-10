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
