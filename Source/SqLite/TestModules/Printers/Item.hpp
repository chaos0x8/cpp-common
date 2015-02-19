#pragma once
#include <SqLite/Item.hpp>
#include <ostream>

namespace Library
{
namespace SqLite
{

inline void PrintTo(const Item& x, std::ostream* os)
{
    (*os) << "{ ";
    (*os) << x.column << ", " << x.value;
    (*os) << " }";

}

}
}
