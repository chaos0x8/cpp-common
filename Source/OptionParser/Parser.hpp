#pragma once

#include "Option.hpp"
#include <memory>
#include <string>
#include <vector>

namespace Common::OptionParser {
  struct Parser {
    using size_type = size_t;
    using value_type = std::string;
    using iterator = std::vector<value_type>::const_iterator;
    using const_iterator = std::vector<value_type>::const_iterator;

    void parse(int argc, const char** argv);
    void parse(std::vector<std::string> args);
    std::string_view name() const;
    std::string help() const;

    size_t size() const {
      return args_.size();
    }

    iterator begin() const {
      return std::cbegin(args_);
    }

    iterator end() const {
      return std::cend(args_);
    }

    std::string_view operator[](size_t index) const {
      return args_[index];
    }

    template <class T> Option<T> on(std::string_view name) {
      auto option = std::make_shared<Detail::TypedOption<T>>(name);
      options_.emplace_back(option);
      return Option<T>(std::move(option));
    }

  private:
    value_type name_;
    std::vector<value_type> args_;
    std::vector<std::weak_ptr<Detail::Option>> options_;
  };
} // namespace Common::OptionParser
