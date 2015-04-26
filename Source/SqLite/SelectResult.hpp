#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <tuple>

namespace Common
{
namespace SqLite
{

class SelectResult
{
public:
    using Row = std::vector<std::string>;
    using size_type = std::vector<Row>::size_type;

    bool empty() const
    {
        return _rows.empty();
    }

    size_type size() const
    {
        return _rows.size();
    }

    void push_back(Row row)
    {
        _rows.push_back(std::move(row));
    }

    void addColumn(std::string column, size_t index)
    {
        _columnMap[column] = index;
    }

    std::vector<Row>& rows()
    {
        return _rows;
    }

    const std::vector<Row>& rows() const
    {
        return _rows;
    }

    std::string& at(size_t index, const std::string& key)
    {
        return _rows[index][_columnMap.at(key)];
    }

    const std::string& at(size_t index, const std::string& key) const
    {
        return _rows[index][_columnMap.at(key)];
    }

    template <class... Args>
    auto mapColumns(Args&&... args) const
    {
        return std::make_tuple(_columnMap.at(args)...);
    }

private:
    std::unordered_map<std::string, size_t> _columnMap;
    std::vector<Row> _rows;
};

}
}
