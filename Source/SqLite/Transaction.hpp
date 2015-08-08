#pragma once

#include <SqLite/ISqLite.hpp>

namespace Common
{
namespace SqLite
{

class Transaction
{
public:
    explicit Transaction(ISqLite& sqLite);
    Transaction(const Transaction&) = delete;

    ~Transaction();

    bool operator = (const Transaction&) = delete;

    void commit();

private:
    ISqLite& sqLite;

    bool commited = false;
};

}
}

