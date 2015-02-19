#pragma once
#include <string>
#include <vector>

namespace Library
{
namespace SqLite
{

class Row;
typedef std::vector<Row> SelectResult;

class ISqLite
{
public:
    virtual ~ISqLite() { }

    virtual void open(const std::string& fileName) = 0;
    virtual SelectResult select(const std::string& select) = 0;
    virtual void execute(const std::string& query) = 0;
    virtual int64_t getLastInsertedId() = 0;

    virtual operator bool() const = 0;
};

}
}
