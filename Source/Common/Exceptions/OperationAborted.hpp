#pragma once

#include <stdexcept>
#include <string>

namespace Common::Exceptions {
  using namespace std::string_literals;

  class OperationAborted : public std::runtime_error {
  public:
    explicit OperationAborted(const std::string& msg)
      : std::runtime_error("OperationAborted '"s + msg + "'"s) {}
  };
} // namespace Common::Exceptions