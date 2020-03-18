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

#include <Network/Selector.hpp>
#include <Network/Pipe.hpp>
#include <gmock/gmock.h>
#include <atomic>

namespace Common
{
namespace Network
{
namespace UT
{

using namespace testing;

class SelectorTestSuite : public Test
{
public:
    void TearDown() override
    {
        auto now = std::chrono::high_resolution_clock::now();
        while (count != expectedCount and now + std::chrono::seconds(1) > std::chrono::high_resolution_clock::now())
            std::this_thread::yield();
        ASSERT_THAT(count, Eq(expectedCount));
    }

    void incrementCount(Pipe* selected)
    {
        std::string data = selected->read();
        count += std::stoul(data);
    }

    void incrementCountTwice(Pipe* selected)
    {
        std::string data = selected->read();
        count += std::stoul(data) * 2;
    }

    std::array<Pipe, 4> pipes;

    Selector sut;

    uint32_t expectedCount{std::numeric_limits<uint32_t>::max()};
    std::atomic<uint32_t> count{0};
};

TEST_F(SelectorTestSuite, shouldCallHandlingProcedure)
{
    expectedCount = 42;

    for (Pipe& pipe : pipes)
        sut.add(pipe, &SelectorTestSuite::incrementCount, this, &pipe);
    pipes[0].write("12");
    pipes[1].write("10");
    pipes[2].write("15");
    pipes[3].write("5");
}

TEST_F(SelectorTestSuite, shouldRemoveElementsOnlyOnce)
{
    expectedCount = 17;

    for (Pipe& pipe : pipes)
        sut.add(pipe, &SelectorTestSuite::incrementCount, this, &pipe);
    sut.remove(pipes[1]);
    sut.remove(pipes[1]);
    sut.remove(pipes[2]);

    pipes[0].write("12");
    pipes[1].write("10");
    pipes[2].write("15");
    pipes[3].write("5");
}

TEST_F(SelectorTestSuite, addingFdTwiceShouldNotWork)
{
    expectedCount = 210;

    sut.add(pipes[0], &SelectorTestSuite::incrementCount, this, &pipes[0]);
    sut.add(pipes[0], &SelectorTestSuite::incrementCountTwice, this, &pipes[0]);
    sut.add(pipes[1], &SelectorTestSuite::incrementCountTwice, this, &pipes[1]);

    pipes[0].write("10");
    pipes[1].write("100");
}

TEST_F(SelectorTestSuite, selectorCanBeSafetlyStoppedManually)
{
  expectedCount = 0;

  sut.stop();
  sut.wait();
}

}
}
}
