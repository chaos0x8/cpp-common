#pragma once
#include <SqLite/Item.hpp>
#include <vector>

namespace Library
{
namespace SqLite
{

class Row
{
public:
    void push_back(const Item&);

    const std::string& operator[](const std::string&) const;

private:
    std::vector<Item> items;
};

}
}
