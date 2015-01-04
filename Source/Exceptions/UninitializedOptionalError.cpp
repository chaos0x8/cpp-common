#include <Exceptions/UninitializedOptionalError.hpp>

namespace Common
{
namespace Exceptions
{

UninitializedOptionalError::UninitializedOptionalError()
    : std::runtime_error("Optional value uninitialized!")
{
}

}
}
