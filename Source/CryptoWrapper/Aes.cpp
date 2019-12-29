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
#include <algorithm>
#include <cassert>

namespace Cryptpad
{
namespace CryptoWrapper
{

void Aes::setPassword(const std::string& password)
{
    setPassword(std::vector<char>(std::begin(password), std::end(password)));
}

void Aes::setPassword(const std::vector<char>& password)
{
    using namespace CryptoPP;

    byte digest[SHA::DIGESTSIZE];
    SHA().CalculateDigest(digest, (const byte*) password.data(), password.size());

    static_assert(sizeof(key) <= sizeof(digest) * 2, "digest size is too small");
    for (size_t i = 0; i < sizeof(key); ++i)
    {
        const size_t digestIndex = (i % 2 == 0 ? i/2 : sizeof(digest) - i/2 - 1);
        assert(digestIndex >= 0 && digestIndex < sizeof(digest));
        key[i] = digest[digestIndex];
    }

    static_assert(sizeof(iv) <= sizeof(digest), "digest size is too small");
    std::copy_n(std::begin(digest) + sizeof(digest) - sizeof(iv), sizeof(iv), std::begin(iv));
}

std::vector<char> Aes::encrypt(const std::string& input) const
{
    return encrypt(std::vector<char>(std::begin(input), std::end(input)));
}

std::vector<char> Aes::encrypt(const std::vector<char>& input) const
{
    using namespace CryptoPP;

    std::vector<char> output(input.size());

    CFB_Mode<AES>::Encryption cfbEncryption(key, sizeof(key), iv, 1);
    cfbEncryption.ProcessData((byte*) output.data(), (const byte*) input.data(), input.size());

    return output;
}

std::vector<char> Aes::decrypt(const std::string& input) const
{
    return decrypt(std::vector<char>(std::begin(input), std::end(input)));
}

std::vector<char> Aes::decrypt(const std::vector<char>& input) const
{
    using namespace CryptoPP;

    std::vector<char> output(input.size());

    CFB_Mode<AES>::Decryption cfbDecryption(key, sizeof(key), iv, 1);
    cfbDecryption.ProcessData((byte*) output.data(), (const byte*) input.data(), input.size());

    return output;
}

}
}
