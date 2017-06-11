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

#include <gmock/gmock.h>
#include <Network/TcpIpServer.hpp>
#include <Network/TcpIpClient.hpp>
#include <Common/Exceptions/SystemError.hpp>
#include <future>

namespace Common
{
  namespace Network
  {
    namespace UT
    {

      using namespace testing;

      class TcpIpSocketsTestSuite : public Test
      {
      public:
        TcpIpSocketsTestSuite()
        {
          acceptResult = std::async(std::launch::async, [&] { return listener.accept(); });
        }

        TcpIpServer listener{LOCAL_HOST, PORT};
        std::future<TcpIpClient> acceptResult;

        static const std::string LOCAL_HOST;
        static const std::string PORT;
      };

      const std::string TcpIpSocketsTestSuite::LOCAL_HOST = "127.0.0.1";
      const std::string TcpIpSocketsTestSuite::PORT = "3042";

      TEST_F(TcpIpSocketsTestSuite, sendReceive)
      {
        TcpIpClient clientClientSide = TcpIpClient{LOCAL_HOST, PORT};
        clientClientSide.send("Hello world");

        TcpIpClient clientServerSide = acceptResult.get();
        ASSERT_THAT(clientServerSide.receive(), Eq("Hello world"));
      }

      TEST_F(TcpIpSocketsTestSuite, shoutdownClient)
      {
        TcpIpClient clientClientSide = TcpIpClient{LOCAL_HOST, PORT};
        TcpIpClient clientServerSide = acceptResult.get();

        clientServerSide.shutdown();

        ASSERT_THAT(clientClientSide.receive(), IsEmpty());
        ASSERT_THAT(clientServerSide.receive(), IsEmpty());
      }

      TEST_F(TcpIpSocketsTestSuite, shutdownListener)
      {
        TcpIpClient clientClientSide = TcpIpClient{LOCAL_HOST, PORT};
        TcpIpClient clientServerSide = acceptResult.get();

        listener.shutdown();
        ASSERT_THROW(listener.accept(), Exceptions::SystemError);
      }

      struct TcpIpSockets_rawMessages_TestSuite : public TcpIpSocketsTestSuite
      {
        struct Message
        {
          bool operator == (const Message& other) const
          {
            return x == other.x and y == other.y;
          }

          int x;
          int y;
        };

        Message msg { 7, 42 };
        Message act {};
      };

      TEST_F(TcpIpSockets_rawMessages_TestSuite, sendReceive)
      {
        TcpIpClient clientClientSide = TcpIpClient{LOCAL_HOST, PORT};
        TcpIpClient clientServerSide = acceptResult.get();

        clientClientSide.send(&msg, sizeof(msg));

        ASSERT_THAT(clientServerSide.receive(&act, sizeof(act)), Eq(true));
        ASSERT_THAT(act, Eq(msg));
      }

      TEST_F(TcpIpSockets_rawMessages_TestSuite, shoutdownClient)
      {
        TcpIpClient clientClientSide = TcpIpClient{LOCAL_HOST, PORT};
        TcpIpClient clientServerSide = acceptResult.get();

        clientServerSide.shutdown();

        ASSERT_THAT(clientClientSide.receive(&act, sizeof(act)), Eq(false));
        ASSERT_THAT(clientServerSide.receive(&act, sizeof(act)), Eq(false));
      }
    }
  }
}
