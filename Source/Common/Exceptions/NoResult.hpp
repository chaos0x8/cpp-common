#pragma once
#include <stdexcept>

namespace Common
{
namespace Exceptions
{

class NoResult : public std::runtime_error
{
public:
    explicit NoResult();
};

}
}

