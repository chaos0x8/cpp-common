#pragma once

#include <stdexcept>

namespace Common::Exceptions {
  class NoResult : public std::runtime_error {
  public:
    explicit NoResult() : std::runtime_error("NoResult") {}
  };
} // namespace Common::Exceptions