#pragma once

#include <stdexcept>

namespace Common
{
namespace Exceptions
{

class OperationAborted : public std::runtime_error
{
public:
    explicit OperationAborted(const std::string& operation);
};

}
}
