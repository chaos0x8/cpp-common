/*!
 *  \author <https://github.com/chaos0x8>
 *  \copyright
 *  Copyright (c) 2015 - 2016, <https://github.com/chaos0x8>
 *
 *  \copyright
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  \copyright
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#pragma once
#include <SqLite/ISqLite.hpp>
#include <sqlite3.h>
#include <string>
#include <memory>
#include <vector>
#include <cstdint>

namespace Common
{
namespace SqLite
{

class SqLite3 : public ISqLite
{
public:
    void open(const std::string& fileName) override;
    SelectResult select(const std::string& select) override;
    void execute(const std::string& query) override;
    int64_t getLastInsertedId() override;

    bool transaction(std::function<void ()> operation) override;

    void setTestMode() override;

    operator bool() const override;

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
