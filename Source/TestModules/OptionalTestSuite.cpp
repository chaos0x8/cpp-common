#include <Optional.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdint>
#include <string>

namespace Common
{

template <typename T>
void PrintTo(const Optional<T>& obj, std::ostream* os)
{
    if (obj.initialized())
        *os << "{ " << obj.value() << " }";
    else
        *os << "{ none }";
}

namespace UT
{

using namespace testing;

TEST(OptionalTestSuite, shouldNotMatchIfNotInitialized)
{
    ASSERT_THAT(Optional<int64_t>(), Not(Eq(0)));
    ASSERT_THAT(Optional<int64_t>(), Not(Eq(42)));

    ASSERT_THAT(Optional<std::string>(), Not(Eq("")));
    ASSERT_THAT(Optional<std::string>(), Not(Eq("hello")));
}

TEST(OptionalTestSuite, shouldNotMatchWhenInitializedWithDifferentValue)
{
    ASSERT_THAT(makeOptional(42), Not(Eq(0)));
    ASSERT_THAT(makeOptional(30), Not(Eq(42)));

    ASSERT_THAT(makeOptional("lol"), Not(Eq("")));
    ASSERT_THAT(makeOptional("helloo"), Not(Eq("hello")));
}

TEST(OptionalTestSuite, shouldNotMatchWithOtherOptionalsWithDifferentValue)
{
    ASSERT_THAT(Optional<int64_t>(none), Not(Eq(Optional<int64_t>(30))));
    ASSERT_THAT(Optional<int64_t>(42), Not(Eq(Optional<int64_t>(none))));
    ASSERT_THAT(Optional<int64_t>(42), Not(Eq(Optional<int64_t>(30))));
    ASSERT_THAT(Optional<int64_t>(0), Not(Eq(Optional<int64_t>(none))));
}

TEST(OptionalTestSuite, shouldMatchWhenValuesAreEqual)
{
    ASSERT_THAT(makeOptional(42), Eq(42));
    ASSERT_THAT(makeOptional("hello"), Eq("hello"));
}

TEST(OptionalTestSuite, shouldMatchWithOtherOptionalsWhenValuesAreEqual)
{
    ASSERT_THAT(Optional<int64_t>(42), Eq(Optional<int64_t>(42)));
}

TEST(OptionalTestSuite, initializationCheck)
{
    Optional<int64_t> cut;
    ASSERT_THAT(cut.initialized(), Eq(false));

    cut = 42;
    ASSERT_THAT(cut.initialized(), Eq(true));

    cut = Optional<int64_t>();
    ASSERT_THAT(cut.initialized(), Eq(false));

    cut = makeOptional(int64_t(42));
    ASSERT_THAT(cut.initialized(), Eq(true));
}

TEST(OptionalTestSuite, valueCheck)
{
    Optional<int64_t> cut(42);

    ASSERT_THAT(cut.value(), Eq(42));
    ASSERT_THAT(*cut, Eq(42));
    ASSERT_THAT(cut.value_or(30), Eq(42));

    cut = Optional<int64_t>();
    ASSERT_THROW(cut.value(), Exceptions::UninitializedOptionalError);
    ASSERT_THROW(*cut, Exceptions::UninitializedOptionalError);
    ASSERT_THAT(cut.value_or(30), Eq(30));
}

TEST(OptionalTestSuite, shouldModifyValue)
{
    Optional<int64_t> cut(42);
    *cut = 27;
    ASSERT_THAT(cut, Eq(27));
}

TEST(OptionalTestSuite, shouldAccessValue)
{
    Optional<std::string> cut("hello");
    ASSERT_THAT(cut->length(), Eq(5));

    cut = none;
    ASSERT_THROW(cut->length(), Exceptions::UninitializedOptionalError);
}

TEST(OptionalTestSuite, resetCheck)
{
    Optional<int64_t> cut(none);
    ASSERT_THAT(cut.initialized(), Eq(false));
    ASSERT_THROW(cut.value(), Exceptions::UninitializedOptionalError);

    cut = 42;
    ASSERT_THAT(cut.initialized(), Eq(true));
    ASSERT_THAT(cut.value(), Eq(42));

    cut = none;
    ASSERT_THAT(cut.initialized(), Eq(false));
    ASSERT_THROW(cut.value(), Exceptions::UninitializedOptionalError);
}

}
}
