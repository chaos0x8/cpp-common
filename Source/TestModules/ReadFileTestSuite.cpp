#include <FileUtility.hpp>
#include <Exceptions/FileLoadError.hpp>
#include <gmock/gmock.h>

namespace Common
{
namespace UT
{

using namespace testing;

TEST(ReadFileTestSuite, shouldThrowWhenFileDoesntExist)
{
    ASSERT_THROW(readFile("xxx"), Exceptions::FileLoadError);
}

TEST(ReadFileTestSuite, shouldReadDataFromFile)
{
    ASSERT_THAT(readFile("Source/TestModules/main.cpp"), Not(IsEmpty()));
}

}
}
