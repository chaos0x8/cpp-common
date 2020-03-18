/*!
 *  \author <https://github.com/chaos0x8>
 *  \copyright
 *  Copyright (c) 2015, <https://github.com/chaos0x8>
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

#include <CryptoWrapper/Md5.hpp>
#include <Common/HexCoding.hpp>
#include <gmock/gmock.h>

namespace Cryptpad
{
namespace CryptoWrapper
{
namespace UT
{

using namespace testing;

class Md5TestSuite : public Test
{
public:
    Md5 sut;

    const std::string TEXT = "Hello world!";
};

MATCHER(Hex, "should be hex")
{
    return arg.size() > 0 and arg.find_first_not_of("0123456789abcdef") == std::string::npos;
}

TEST_F(Md5TestSuite, returnsMd5String)
{
    ASSERT_THAT(sut.encrypt(TEXT), Hex());
}

TEST_F(Md5TestSuite, resultIsDeterministic)
{
    ASSERT_THAT(sut.encrypt(TEXT), Eq(sut.encrypt(TEXT)));
}

TEST_F(Md5TestSuite, resultIsDifferentThanInput)
{
    ASSERT_THAT(sut.encrypt(TEXT), Not(Eq(TEXT)));
}

TEST_F(Md5TestSuite, resultIsEncrypted)
{
    using Common::HexCoding::encrypt;

    ASSERT_THAT(sut.encrypt(TEXT), Not(Eq(encrypt(TEXT))));
}

}
}
}
