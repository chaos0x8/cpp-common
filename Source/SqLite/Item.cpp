#include <SqLite/Item.hpp>

namespace Library
{
namespace SqLite
{

Item::Item(const char* column, const char* value)
    : column(column ? column : std::string()),
      value(value ? value : std::string())
{
}

}
}
