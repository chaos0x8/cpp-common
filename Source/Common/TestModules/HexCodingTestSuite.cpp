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

#include <Common/HexCoding.hpp>
#include <gmock/gmock.h>

namespace Common
{
namespace HexCoding
{
namespace UT
{

using namespace testing;

class HexCodingTestSuite : public Test
{
public:
    using V = std::vector<char>;

    static std::string binaryString();

    const std::string TEXT = "a cat";
};

TEST_F(HexCodingTestSuite, encodeCharacters)
{
    ASSERT_THAT(encrypt(V{'0', 28, 30}), Eq("301c1e"));
}

TEST_F(HexCodingTestSuite, encodeCharactersWithPadding)
{
    ASSERT_THAT(encrypt(V{7, 15, 12}), Eq("070f0c"));
}

TEST_F(HexCodingTestSuite, decodesCharacters)
{
    ASSERT_THAT(decrypt("305f6e"), Eq("0_n"));
}

TEST_F(HexCodingTestSuite, decodesEncodedData)
{
    const std::string encoded = encrypt(TEXT);

    ASSERT_THAT(decrypt(encoded), Eq(TEXT));
}

std::string HexCodingTestSuite::binaryString()
{
    std::vector<char> binary(256);
    for (size_t i = 0; i < binary.size(); ++i)
        binary[i] = i;
    return std::string(binary.data(), binary.size());
}

TEST_F(HexCodingTestSuite, decodesEncodedBinaryData)
{
    const std::string encoded = encrypt(binaryString());

    ASSERT_THAT(decrypt(encoded), Eq(binaryString()));
}

}
}
}
