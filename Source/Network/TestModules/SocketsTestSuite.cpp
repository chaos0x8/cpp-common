#include <gmock/gmock.h>
#include <Network/ListeningSocket.hpp>
#include <Network/ClientSocket.hpp>
#include <Parallel/ThreadPool.hpp>

namespace Common
{
namespace Network
{
namespace UT
{

using namespace testing;

class SocketsTestSuite : public Test
{
public:
    SocketsTestSuite()
    {
        acceptResult = pool.queueFuture([&] { return listener.accept(); });
    }

    Parallel::ThreadPool pool{4};

    ListeningSocket listener{LOCAL_HOST, PORT};
    std::future<ClientSocket> acceptResult;
    ClientSocket clientServerSide;
    ClientSocket clientClientSide;

    static const std::string LOCAL_HOST;
    static const std::string PORT;
};

const std::string SocketsTestSuite::LOCAL_HOST = "127.0.0.1";
const std::string SocketsTestSuite::PORT = "3042";

TEST_F(SocketsTestSuite, shouldReceive)
{
    clientClientSide = ClientSocket{LOCAL_HOST, PORT};
    clientClientSide.send("Hello world");

    clientServerSide = acceptResult.get();
    ASSERT_THAT(clientServerSide.receive(), Eq("Hello world"));
}

}
}
}
