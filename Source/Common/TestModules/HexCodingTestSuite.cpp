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
    ASSERT_THAT(encode(V{'0', 28, 30}), Eq("301c1e"));
}

TEST_F(HexCodingTestSuite, encodeCharactersWithPadding)
{
    ASSERT_THAT(encode(V{7, 15, 12}), Eq("070f0c"));
}

TEST_F(HexCodingTestSuite, decodesCharacters)
{
    ASSERT_THAT(decode("305f6e"), Eq("0_n"));
}

TEST_F(HexCodingTestSuite, decodesEncodedData)
{
    const std::string encoded = encode(TEXT);

    ASSERT_THAT(decode(encoded), Eq(TEXT));
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
    const std::string encoded = encode(binaryString());

    ASSERT_THAT(decode(encoded), Eq(binaryString()));
}

}
}
}
