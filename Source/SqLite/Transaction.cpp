#include <SqLite/Transaction.hpp>

namespace Common
{
namespace SqLite
{

Transaction::Transaction(ISqLite& sqLite)
    : sqLite(sqLite)
{
    this->sqLite.execute("begin transaction");
}

Transaction::~Transaction()
{
    if (!commited)
        sqLite.execute("rollback");
}

void Transaction::commit()
{
    sqLite.execute("commit transaction");
    commited = true;
}

}
}
