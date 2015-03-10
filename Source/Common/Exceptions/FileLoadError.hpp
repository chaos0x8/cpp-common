#pragma once
#include <stdexcept>

namespace Common
{
namespace Exceptions
{

class FileLoadError : public std::runtime_error
{
public:
    explicit FileLoadError(const std::string&);
};

}
}
