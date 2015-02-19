#include <SqLite/Exception.hpp>
#include <boost/format.hpp>

namespace Library
{
namespace SqLite
{

Exception::Exception(const std::string& text)
    : std::runtime_error(boost::str(boost::format("Exception: %1%") % text))
{
}

}
}
