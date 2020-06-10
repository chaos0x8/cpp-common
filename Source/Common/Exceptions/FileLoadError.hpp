#pragma once

#include <stdexcept>
#include <string>

namespace Common::Exceptions {
  using namespace std::string_literals;

  class FileLoadError : public std::runtime_error {
  public:
    explicit FileLoadError(const std::string& msg)
      : std::runtime_error("FileLoadError '"s + msg + "'"s) {}
  };
} // namespace Common::Exceptions