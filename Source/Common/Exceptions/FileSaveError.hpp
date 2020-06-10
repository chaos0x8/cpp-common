#pragma once

#include <stdexcept>
#include <string>

namespace Common::Exceptions {
  using namespace std::string_literals;

  class FileSaveError : public std::runtime_error {
  public:
    explicit FileSaveError(const std::string& msg)
      : std::runtime_error("FileSaveError '"s + msg + "'"s) {}
  };
} // namespace Common::Exceptions