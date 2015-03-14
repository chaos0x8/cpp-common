#include <gmock/gmock.h>
#include <Network/UdpServer.hpp>
#include <Network/UdpClient.hpp>
#include <Parallel/ThreadPool.hpp>

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
    static const std::string LOCAL_HOST;
    static const std::string PORT;
};

const std::string UdpSocketsTestSuite::LOCAL_HOST = "127.0.0.1";
const std::string UdpSocketsTestSuite::PORT = "3042";

TEST_F(UdpSocketsTestSuite, socketTest)
{
    UdpServer server{LOCAL_HOST, PORT};
    UdpClient client{LOCAL_HOST, PORT};

    client.send("Hello world");
    UdpMessage msg = server.receive();
    server.send(msg.clone("Hello back"));
    ASSERT_THAT(msg.data, Eq("Hello world"));
    ASSERT_THAT(client.receive(), Eq("Hello back"));

    UdpHost host = msg.getHost();
    ASSERT_THAT(host.name, Eq("localhost"));
    ASSERT_THAT(host.service, Not(IsEmpty()));
}

}
}
}

