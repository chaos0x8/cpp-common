#pragma once
#include <stdexcept>
#include <string>

namespace Library
{
namespace SqLite
{

class Exception : public std::runtime_error
{
public:
    explicit Exception(const std::string&);
};

}
}
