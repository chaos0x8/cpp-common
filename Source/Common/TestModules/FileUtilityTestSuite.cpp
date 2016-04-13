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

#include <Common/FileUtility.hpp>
#include <Common/Exceptions/FileLoadError.hpp>
#include <gmock/gmock.h>

namespace Common
{
namespace UT
{

using namespace testing;

class FileUtilityTestSuite : public Test
{
public:
    std::string NON_EXISTING_FILE{"xxx"};
    std::string EXISTING_FILE{"Source/Common/TestModules/main.cpp"};
};

TEST_F(FileUtilityTestSuite, shouldThrowWhenFileDoesntExist)
{
    ASSERT_THROW(readFile(NON_EXISTING_FILE), Exceptions::FileLoadError);
}

TEST_F(FileUtilityTestSuite, shouldReadDataFromFile)
{
    ASSERT_THAT(readFile(EXISTING_FILE), Not(IsEmpty()));
}

TEST_F(FileUtilityTestSuite, testBasename)
{
    EXPECT_THAT(basename("filename.ext"), Eq("filename.ext"));
    EXPECT_THAT(basename("directory/filename.ext"), Eq("filename.ext"));
}

TEST_F(FileUtilityTestSuite, testNoExtension)
{
    EXPECT_THAT(noExtension("file.ext"), Eq("file"));
    EXPECT_THAT(noExtension("file"), Eq("file"));
    EXPECT_THAT(noExtension("path.d/file"), Eq("path.d/file"));
}

class FileSizeTest : public FileUtilityTestSuite
{
public:
    void SetUp() override
    {
        stream << STREAM_DATA;
    }

    std::string STREAM_DATA{"Hello"};
    std::stringstream stream;
};

TEST_F(FileSizeTest, returnsSizeOfStream)
{
    ASSERT_THAT(fileSize(stream), Eq(STREAM_DATA.size()));
}

TEST_F(FileSizeTest, preservesCurrentPossition)
{
    stream.seekg(2u, std::ios::beg);

    fileSize(stream);

    ASSERT_THAT(stream.tellg(), Eq(2u));
}

TEST_F(FileSizeTest, throwsWhenFileDoestExists)
{
    ASSERT_THROW(fileSize(NON_EXISTING_FILE), Exceptions::FileLoadError);
}

TEST_F(FileSizeTest, returnFileSize)
{
    ASSERT_THAT(fileSize(EXISTING_FILE), Gt(0u));
}

}
}
