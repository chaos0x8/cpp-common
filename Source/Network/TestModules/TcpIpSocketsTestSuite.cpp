#include <gmock/gmock.h>
#include <Network/TcpIpServer.hpp>
#include <Network/TcpIpClient.hpp>
#include <Parallel/ThreadPool.hpp>

namespace Common
{
namespace Network
{
namespace UT
{

using namespace testing;

class IcpIpSocketsTestSuite : public Test
{
public:
    IcpIpSocketsTestSuite()
    {
        acceptResult = pool.queueFuture([&] { return listener.accept(); });
    }

    Parallel::ThreadPool pool{4};

    TcpIpServer listener{LOCAL_HOST, PORT};
    std::future<TcpIpClient> acceptResult;
    TcpIpClient clientServerSide;
    TcpIpClient clientClientSide;

    static const std::string LOCAL_HOST;
    static const std::string PORT;
};

const std::string IcpIpSocketsTestSuite::LOCAL_HOST = "127.0.0.1";
const std::string IcpIpSocketsTestSuite::PORT = "3042";

TEST_F(IcpIpSocketsTestSuite, socketTest)
{
    clientClientSide = TcpIpClient{LOCAL_HOST, PORT};
    clientClientSide.send("Hello world");

    clientServerSide = acceptResult.get();
    ASSERT_THAT(clientServerSide.receive(), Eq("Hello world"));
}

}
}
}
