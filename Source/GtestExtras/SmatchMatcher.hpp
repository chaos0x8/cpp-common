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

#pragma once

#include <regex>
#include <sstream>
#include <gmock/gmock.h>

namespace Common
{
namespace UT
{

using namespace testing;

namespace Detail
{

class SmatchMatcher
{
public:
    SmatchMatcher(const std::string& regex)
        : regex(regex)
    {
    }

    template <class T>
    bool MatchAndExplain(const T& obj, MatchResultListener*) const
    {
        std::stringstream ss;
        ss << obj;
        return std::regex_match(ss.str(), std::regex(regex));
    }

    void DescribeTo(std::ostream* out) const
    {
        *out << "to match regex '" << regex << "'";
    }

    void DescribeNegationTo(std::ostream* out) const
    {
        *out << "not ";
        DescribeTo(out);
    }

private:
    std::string regex;
};

}

inline PolymorphicMatcher<Detail::SmatchMatcher> Smatch(const std::string& regex)
{
    return MakePolymorphicMatcher(Detail::SmatchMatcher(regex));
}

}
}
