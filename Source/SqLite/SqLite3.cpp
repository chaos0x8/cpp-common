/*!
    \author <https://github.com/chaos0x8>
    \copyright
    Copyright (c) 2015, <https://github.com/chaos0x8>

    \copyright
    Permission to use, copy, modify, and/or distribute this software for any
    purpose with or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    \copyright
    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <SqLite/SqLite3.hpp>
#include <SqLite/Exception.hpp>
#include <SqLite/Item.hpp>
#include <SqLite/Row.hpp>
#include <boost/format.hpp>

namespace Common
{
namespace SqLite
{

void SqLite3::SqLiteDeleter::operator()(sqlite3* db) const
{
    sqlite3_close(db);
}

void SqLite3::SqLiteDeleter::operator()(void* mem) const
{
    sqlite3_free(mem);
}

void SqLite3::open(const std::string& fileName)
{
    sqlite3* tmpDb = 0;
    int err = sqlite3_open(fileName.c_str(), &tmpDb);
    db = std::unique_ptr<sqlite3, SqLiteDeleter>(tmpDb, SqLiteDeleter());

    if (err != SQLITE_OK || operator bool() == false)
    {
        throw Exception(boost::str(boost::format("Cannot read data base '%1%', error code: %2%") % fileName % err));
    }
}

SelectResult SqLite3::select(const std::string& select)
{
    SelectResult result = SelectResult();

    char* rawErrorMsg = 0;
    int err = sqlite3_exec(db.get(), select.c_str(), SqLite3::selectCallBack, &result, &rawErrorMsg);
    if (err != SQLITE_OK)
    {
        std::unique_ptr<char, SqLiteDeleter> errorMsg(rawErrorMsg, SqLiteDeleter());
        throw Exception(boost::str(boost::format("Error during select[%1%]: '%2%'") % err % errorMsg.get()));
    }

    return result;
}

void SqLite3::execute(const std::string& query)
{
    char* rawErrorMsg = 0;
    int err = sqlite3_exec(db.get(), query.c_str(), 0, 0, &rawErrorMsg);
    if (err != SQLITE_OK)
    {
        std::unique_ptr<char, SqLiteDeleter> errorMsg(rawErrorMsg, SqLiteDeleter());
        throw Exception(boost::str(boost::format("Error during execute[%1%]: '%2%'") % err % errorMsg.get()));
    }
}

int64_t SqLite3::getLastInsertedId()
{
    return sqlite3_last_insert_rowid(db.get());
}

SqLite3::operator bool() const
{
    return static_cast<bool>(db);
}

int SqLite3::selectCallBack(void* data, int argc, char** argv, char** colName)
{
    SelectResult* result = static_cast<SelectResult*>(data);

    Row newRow;

    for (int i = 0; i < argc; ++i)
    {
        newRow.push_back(Item(colName[i], argv[i]));
    }

    result->push_back(newRow);

    return 0;
}

}
}
