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

#include <gmock/gmock.h>
#include <Network/UdpServer.hpp>
#include <Network/UdpClient.hpp>
#include <Common/Exceptions/SystemError.hpp>

namespace Common
{
namespace Network
{
namespace UT
{

using namespace testing;

class UdpSocketsTestSuite : public Test
{
public:
    UdpServer server{LOCAL_HOST, PORT};
    UdpClient client{LOCAL_HOST, PORT};

    static const std::string LOCAL_HOST;
    static const std::string PORT;
};

const std::string UdpSocketsTestSuite::LOCAL_HOST = "127.0.0.1";
const std::string UdpSocketsTestSuite::PORT = "3042";

TEST_F(UdpSocketsTestSuite, sendReceive)
{
    client.send("Hello world");
    UdpMessage msg = server.receive();
    server.send(msg.clone("Hello back"));
    ASSERT_THAT(msg.data, Eq("Hello world"));
    ASSERT_THAT(client.receive(), Eq("Hello back"));

    UdpHost host = msg.getHost();
    ASSERT_THAT(host.name, Eq("localhost"));
    ASSERT_THAT(host.service, Not(IsEmpty()));
}

TEST_F(UdpSocketsTestSuite, shutdownClient)
{
    client.shutdown();

    ASSERT_THAT(client.receive(), IsEmpty());
}

}
}
}
