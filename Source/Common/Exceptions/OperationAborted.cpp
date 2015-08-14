#include <Common/Exceptions/OperationAborted.hpp>
#include <boost/format.hpp>

namespace Common
{
namespace Exceptions
{

OperationAborted::OperationAborted(const std::string& operation)
    : std::runtime_error(boost::str(boost::format("OperationAborted: '%1%'") % operation))
{
}

}
}
