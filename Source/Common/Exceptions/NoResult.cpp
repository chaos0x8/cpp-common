#include <Common/Exceptions/NoResult.hpp>
#include <boost/format.hpp>

namespace Common
{
namespace Exceptions
{

NoResult::NoResult()
  : std::runtime_error(boost::str(boost::format("NoResult")))
{
}

}
}

