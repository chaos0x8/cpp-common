#include "OptionParser/Parser.hpp"
#include <gmock/gmock.h>

struct Foo {
  static Foo fromString(std::string_view str) {
    auto ret = Foo{};
    ret.a = std::stoll(std::string(str));
    ret.b = str.size();
    return ret;
  }

  int a;
  int b;
};

namespace Common::OptionParser {
  using namespace testing;

  struct ParserTestSuite : public Test {
    Parser sut;
  };

  TEST_F(ParserTestSuite, accessBooleanParameterNotPresent) {
    auto help = sut.on<bool>("--help");

    sut.parse({});

    ASSERT_THAT(static_cast<bool>(help), Eq(false));
    EXPECT_THAT(help, AllOf(Ne(false), Ne(true)));
    EXPECT_THAT(sut, SizeIs(0u));
  }

  TEST_F(ParserTestSuite, accessBooleanParameterPresent) {
    auto help = sut.on<bool>("--help");

    sut.parse({"--help"});

    ASSERT_THAT(static_cast<bool>(help), Eq(true));
    EXPECT_THAT(*help, Eq(true));
    EXPECT_THAT(help, Eq(true));
    EXPECT_THAT(sut, SizeIs(0u));
  }

  TEST_F(ParserTestSuite, accessStringParameterNotPresent) {
    auto name = sut.on<std::string>("--name");

    sut.parse({});

    ASSERT_THAT(static_cast<bool>(name), Eq(false));
    EXPECT_THAT(sut, SizeIs(0u));
  }

  TEST_F(ParserTestSuite, accessStringParameterPresent) {
    auto name = sut.on<std::string>("--name");

    sut.parse({"--name", "Lara"});

    ASSERT_THAT(static_cast<bool>(name), Eq(true));
    EXPECT_THAT(*name, Eq("Lara"));
    EXPECT_THAT(name, Eq("Lara"));
    EXPECT_THAT(sut, SizeIs(0u));
  }

  TEST_F(ParserTestSuite, accessUnsignedParameterPresent) {
    auto age = sut.on<uint8_t>("--age");
    auto height = sut.on<uint32_t>("--height");

    sut.parse({"--age", "42", "--height", "128"});

    EXPECT_THAT(age, Eq(42u));
    EXPECT_THAT(height, Eq(128u));
    EXPECT_THAT(sut, SizeIs(0u));
  }

  TEST_F(ParserTestSuite, accessSignedParameterPresent) {
    auto temp = sut.on<int8_t>("--temperature");
    auto money = sut.on<int32_t>("--money");

    sut.parse({"--temperature", "-10", "--money", "-42"});

    EXPECT_THAT(temp, Eq(-10));
    EXPECT_THAT(money, Eq(-42));
    EXPECT_THAT(sut, SizeIs(0u));
  }

  TEST_F(ParserTestSuite, accessFloatingPointParameterPresent) {
    auto temp = sut.on<float>("--temperature");
    auto money = sut.on<double>("--money");

    sut.parse({"--temperature", "-10.7", "--money", "42.8"});

    EXPECT_THAT(temp, Eq(-10.7f));
    EXPECT_THAT(money, Eq(42.8));
    EXPECT_THAT(sut, SizeIs(0u));
  }

  TEST_F(ParserTestSuite, accessCustomClassParameterPresent) {
    auto foo = sut.on<Foo>("--foo");

    sut.parse({"--foo", "142"});

    EXPECT_THAT(foo->a, Eq(142));
    EXPECT_THAT(foo->b, Eq(3));
    EXPECT_THAT(sut, SizeIs(0u));
  }

  TEST_F(ParserTestSuite, accessLambdaParameterPresentWithoutArg) {
    bool help = false;

    sut.on<0>("--help", [&help] { help = true; });
    sut.parse({"--help"});

    ASSERT_THAT(help, Eq(true));
  }

  TEST_F(ParserTestSuite, accessLambdaParameterNotPresentWithoutArg) {
    bool help = false;

    sut.on<0>("--help", [&help] { help = true; });
    sut.parse({});

    ASSERT_THAT(help, Eq(false));
  }

  TEST_F(ParserTestSuite, accessLambdaParameterPresentWithArg) {
    std::string help;

    sut.on<1>("--help", [&help](std::string_view arg) { help = arg; });
    sut.parse({"--help", "me"});

    ASSERT_THAT(help, Eq("me"));
  }

  TEST_F(ParserTestSuite, accessLambdaParameterNotPresentWithArg) {
    std::string help;

    sut.on<1>("--help", [&help](std::string_view arg) { help = arg; });
    sut.parse({});

    ASSERT_THAT(help, Eq(""));
  }

  struct ParserArgsAccessTestSuite : public ParserTestSuite {
    void SetUp() override {
      std::array<const char*, 5> args = {"app", "a0", "a1", "a2", "a3"};
      sut.parse(4, args.data());
    }
  };

  TEST_F(ParserArgsAccessTestSuite, accessElements) {
    EXPECT_THAT(sut, ElementsAre("a0", "a1", "a2"));
  }

  TEST_F(ParserArgsAccessTestSuite, randomAccessElements) {
    ASSERT_THAT(sut, SizeIs(3));
    EXPECT_THAT(sut[0], "a0");
    EXPECT_THAT(sut[1], "a1");
    EXPECT_THAT(sut[2], "a2");
  }

  struct ParserHelpTestSuite : public ParserTestSuite {
    std::array<const char*, 1> args = {"appName"};

    Option<std::string> name = sut.on<std::string>("--name");
    Option<bool> help = sut.on<bool>("--help");
  };

  TEST_F(ParserHelpTestSuite, returnsHelpText) {
    std::string e;
    e += "Usage: [options] args...\n";
    e += "\n";
    e += "--name\n";
    e += "--help\n";

    EXPECT_THAT(sut.help(), Eq(e));
  }

  TEST_F(ParserHelpTestSuite, returnsHelpTextWithAppName) {
    sut.parse(args.size(), args.data());

    std::string e;
    e += "Usage: appName [options] args...\n";
    e += "\n";
    e += "--name\n";
    e += "--help\n";

    EXPECT_THAT(sut.help(), Eq(e));
  }
} // namespace Common::OptionParser
