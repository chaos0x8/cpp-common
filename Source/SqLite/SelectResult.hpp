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

#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <tuple>

namespace Common
{
namespace SqLite
{

struct FirstLetterHash
{
    size_t operator()(const std::string& text) const
    {
        return text[0];
    }
};

class SelectResult
{
public:
    using Row = std::vector<std::string>;
    using size_type = std::vector<Row>::size_type;

    bool empty() const
    {
        return rows.empty();
    }

    size_type size() const
    {
        return rows.size();
    }

    void push_back(Row row)
    {
        rows.push_back(std::move(row));
    }

    void addColumn(const std::string& column, size_t index)
    {
        _columnMap[column] = index;
    }

    std::string& at(size_t index, const std::string& key)
    {
        return rows[index][_columnMap.at(key)];
    }

    const std::string& at(size_t index, const std::string& key) const
    {
        return rows[index][_columnMap.at(key)];
    }

    template <class... Args>
    auto mapColumns(Args&&... args) const
    {
        return std::make_tuple(_columnMap.at(args)...);
    }

    std::vector<Row> rows;

private:
    std::unordered_map<std::string, size_t, FirstLetterHash> _columnMap;
};

}
}
