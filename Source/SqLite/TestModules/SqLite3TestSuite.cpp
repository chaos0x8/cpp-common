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

#include <SqLite/SqLite3.hpp>
#include <SqLite/DBCast.hpp>
#include <boost/format.hpp>
#include <gmock/gmock.h>

namespace Common
{
namespace SqLite
{

using namespace testing;

struct Field
{
    std::string value;

    bool operator == (const Field& other) const
    {
        return value == other.value;
    }
};

class SqLite3TestSuite : public Test
{
public:
    static void SetUpTestCase()
    {
        sut = std::unique_ptr<SqLite3>(new SqLite3);
        sut->open(DB_FILE);

        sut->setTestMode();
        sut->execute("create table field(value string);");
    }

    void TearDown() override
    {
        assert(sut != nullptr);

        std::for_each(std::begin(fieldToDelete), std::end(fieldToDelete),
            std::bind(&SqLite3TestSuite::deleteField, std::placeholders::_1));
    }

    static void TearDownTestCase()
    {
        sut = nullptr;

        system(boost::str(boost::format(
            "if [ -e %1% ]; then rm -v %1%; fi")
            % DB_FILE).c_str());
    }

    void insertField(const Field& field, bool deleteAfter = true)
    {
        sut->execute(boost::str(boost::format(
            "insert into field (value) values(%1%)")
            % DBCast::toDBFormat(field.value)));

        if (deleteAfter)
            fieldToDelete.push_back(field);
    }

    static void deleteField(const Field& field)
    {
        sut->execute(boost::str(boost::format(
            "delete from field where value = %1%")
            % DBCast::toDBFormat(field.value)));
    }

    static std::vector<Field> loadFields()
    {
        SelectResult sl = sut->select("select * from field");

        std::vector<Field> result;
        std::transform(std::begin(sl.rows), std::end(sl.rows), std::back_inserter(result),
            [](const SelectResult::Row& row)
            {
                constexpr size_t VALUE_INDEX = 0;
                return Field{ DBCast::fromDBFormat<std::string>(row[VALUE_INDEX]) };
            });
        return result;
    }

    const Field FIELD_1{"cat"};
    const Field FIELD_2{"dog"};
    const Field FIELD_3{"tiger"};

    static const std::string DB_FILE;

    static std::unique_ptr<SqLite3> sut;

private:
    std::vector<Field> fieldToDelete;
};

const std::string SqLite3TestSuite::DB_FILE = "test.db";
std::unique_ptr<SqLite3> SqLite3TestSuite::sut;

TEST_F(SqLite3TestSuite, selectsValue)
{
    insertField(FIELD_1);

    ASSERT_THAT(loadFields(), UnorderedElementsAre(FIELD_1));
}

TEST_F(SqLite3TestSuite, sucessfulTransaction)
{
    const bool transactionStatus = sut->transaction(
        [this] () -> void
        {
            insertField(FIELD_1);
            insertField(FIELD_2);
        });

    ASSERT_TRUE(transactionStatus);
    ASSERT_THAT(loadFields(), UnorderedElementsAre(FIELD_1, FIELD_2));
}

TEST_F(SqLite3TestSuite, failedTransaction)
{
    insertField(FIELD_1);

    const bool transactionStatus = sut->transaction(
        [this] () -> void
        {
            insertField(FIELD_2, false);
            insertField(FIELD_3, false);
            sut->execute("insert into not_existing (field) ('value')");
        });

    ASSERT_FALSE(transactionStatus);
    ASSERT_THAT(loadFields(), UnorderedElementsAre(FIELD_1));
}

TEST_F(SqLite3TestSuite, failedTransactionWithUserException)
{
    insertField(FIELD_1);

    const bool transactionStatus = sut->transaction(
        [this] () -> void
        {
            insertField(FIELD_2, false);
            insertField(FIELD_3, false);
            throw std::exception();
        });

    ASSERT_FALSE(transactionStatus);
    ASSERT_THAT(loadFields(), UnorderedElementsAre(FIELD_1));
}

}
}
