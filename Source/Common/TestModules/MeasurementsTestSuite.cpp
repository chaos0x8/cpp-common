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

#include <Common/Measurements.hpp>
#include <GtestExtras/SmatchMatcher.hpp>
#include <sstream>
#include <thread>
#include <chrono>
#include <boost/format.hpp>
#include <gmock/gmock.h>

namespace Common
{
namespace UT
{

using namespace testing;
using namespace std::chrono_literals;

class MeasurementsTestSuite : public Test
{
public:
    const std::string TITLE = "title";

    std::stringstream output;
};

void testFunction(std::chrono::milliseconds time)
{
    std::this_thread::sleep_for(time);
}

TEST_F(MeasurementsTestSuite, measuresFastLambdaExecutionTime)
{
    measureTime(TITLE, output, []{});

    ASSERT_THAT(output.str(), Smatch(boost::str(boost::format("%1% took \\d ms\\n") % TITLE)));
}

TEST_F(MeasurementsTestSuite, measuresSlowLambdaExecutionTime)
{
    measureTime(TITLE, output, []{ std::this_thread::sleep_for(10ms); });

    ASSERT_THAT(output.str(), Smatch(boost::str(boost::format("%1% took \\d{2} ms\\n") % TITLE)));
}

TEST_F(MeasurementsTestSuite, measuresFastFunctionExecutionTime)
{
    measureTime(TITLE, output, &testFunction, 0ms);

    ASSERT_THAT(output.str(), Smatch(boost::str(boost::format("%1% took \\d ms\\n") % TITLE)));
}

TEST_F(MeasurementsTestSuite, measuresSlowFunctionExecutionTime)
{
    measureTime(TITLE, output, &testFunction, 10ms);

    ASSERT_THAT(output.str(), Smatch(boost::str(boost::format("%1% took \\d{2} ms\\n") % TITLE)));
}

}
}