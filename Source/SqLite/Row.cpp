#include <SqLite/Row.hpp>
#include <algorithm>
#include <cassert>
#include <iostream>

namespace Library
{
namespace SqLite
{

void Row::push_back(const Item& item)
{
    items.push_back(item);
}

const std::string& Row::operator[](const std::string& index) const
{
    std::vector<Item>::const_iterator it = find_if(items.begin(), items.end(),
    [&index](const Item& item)
    {
        return item.column == index;
    });

    assert(it != items.end());

    return it->value;
}

}
}
