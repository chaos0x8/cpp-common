#pragma once
#include <stdexcept>

namespace Common
{
namespace Exceptions
{

class UninitializedOptionalError : public std::runtime_error
{
public:
    explicit UninitializedOptionalError();
};

}
}
