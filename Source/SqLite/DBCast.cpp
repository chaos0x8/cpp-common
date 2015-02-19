#include <SqLite/DBCast.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string/replace.hpp>

namespace Library
{
namespace SqLite
{
namespace DBCast
{

std::string toDBFormat(const std::string& value)
{
    return boost::str(boost::format("'%1%'") % boost::replace_all_copy(value, "'", "''"));
}

std::string toDBFormat(const char* value)
{
    return toDBFormat(std::string(value));
}

}
}
}
