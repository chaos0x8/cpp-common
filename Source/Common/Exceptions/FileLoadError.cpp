#include <Common/Exceptions/FileLoadError.hpp>
#include <boost/format.hpp>

namespace Common
{
namespace Exceptions
{

FileLoadError::FileLoadError(const std::string& fileName)
    : std::runtime_error(boost::str(boost::format("Could not read from file: %1%") % fileName))
{
}

}
}
