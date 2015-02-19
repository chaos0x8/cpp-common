#pragma once
#include <string>

namespace Library
{
namespace SqLite
{

class Item
{
public:
    Item(const char* column, const char* value);

    std::string column;
    std::string value;
};

}
}
