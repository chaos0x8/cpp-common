#pragma once

#include <stdexcept>
#include <string>

namespace Common::Exceptions {
  using namespace std::string_literals;

  class AssertionFailed : public std::runtime_error {
  public:
    explicit AssertionFailed(const std::string& msg)
      : std::runtime_error("AssertionFailed '"s + msg + "'"s) {}
  };
} // namespace Common::Exceptions