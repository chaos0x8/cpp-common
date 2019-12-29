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

#include <CryptoWrapper/Aes.hpp>
#include <gmock/gmock.h>

namespace Cryptpad
{
namespace CryptoWrapper
{
namespace UT
{

using namespace testing;

class AesTestSuite : public Test
{
public:
    Aes sut;

    static std::vector<char> vec(const std::string& s)
    {
        return std::vector<char>(std::begin(s), std::end(s));
    }

    const std::string PASSWORD = "password";
    const std::string TEXT = "hello world!";
};

TEST_F(AesTestSuite, encryptGivesDeterministicResult)
{
    ASSERT_THAT(sut.encrypt(vec(TEXT)), Not(ElementsAreArray(vec(TEXT))));
    ASSERT_THAT(sut.encrypt(TEXT), Not(ElementsAreArray(vec(TEXT))));
}

TEST_F(AesTestSuite, encryptGivesDifferentResultWhenPasswordIsChanged)
{
    const std::vector<char> encrypted = sut.encrypt(TEXT);
    sut.setPassword(PASSWORD);

    ASSERT_THAT(encrypted, Not(ElementsAreArray(sut.encrypt(TEXT))));
}

TEST_F(AesTestSuite, decryptGivesDataSameAsBeforeEncryption)
{
    const std::vector<char> encrypted = sut.encrypt(TEXT);

    ASSERT_THAT(sut.decrypt(encrypted), ElementsAreArray(vec(TEXT)));
}

TEST_F(AesTestSuite, decryptGivesDifferentDataWhenPasswordWasChanged)
{
    sut.setPassword(PASSWORD);
    const std::vector<char> encrypted = sut.encrypt(TEXT);
    sut.setPassword(std::string("wrong") + PASSWORD);

    ASSERT_THAT(sut.decrypt(encrypted), Not(ElementsAreArray(vec(TEXT))));
}

}
}
}
