#pragma once
#include <SqLite/ISqLite.hpp>
#include <sqlite3.h>
#include <string>
#include <memory>
#include <vector>
#include <cstdint>

namespace Library
{
namespace SqLite
{

class SqLite3 : public ISqLite
{
public:
    void open(const std::string& fileName);
    SelectResult select(const std::string& select);
    void execute(const std::string& query);
    int64_t getLastInsertedId();

    operator bool() const;

private:
    class SqLiteDeleter
    {
    public:
        void operator()(sqlite3* db) const;
        void operator()(void* mem) const;
    };

    static int selectCallBack(void* data, int argc, char** argv, char** colName);

    std::unique_ptr<sqlite3, SqLiteDeleter> db;
};

}
}
