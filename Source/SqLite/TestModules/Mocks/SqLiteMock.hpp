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
#include <gmock/gmock.h>

namespace Common
{
namespace SqLite
{
namespace UT
{

class SqLiteMock : public ISqLite
{
public:
    MOCK_METHOD1(open, void(const std::string&));
    MOCK_METHOD1(select, SelectResult(const std::string&));
    MOCK_METHOD1(execute, void(const std::string&));
    MOCK_METHOD0(getLastInsertedId, int64_t());

    MOCK_METHOD2(transaction, bool(std::function<void ()>, std::ostream&));

    MOCK_METHOD0(setTestMode, void());

    operator bool() const override
    {
        return operatorBool();
    }

    MOCK_CONST_METHOD0(operatorBool, bool());
};

}
}
}
