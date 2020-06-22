#pragma once

#include "Common/Eql.hpp"
#include <memory>
#include <optional>
#include <string>
#include <type_traits>

namespace Common::OptionParser {
  namespace Detail {
    template <class T> struct Traits {
      static T fromString(std::string_view str) {
        if constexpr (std::is_floating_point_v<T>) {
          return std::stod(std::string(str));
        } else if constexpr (std::is_signed_v<T>) {
          return std::stoll(std::string(str));
        } else if constexpr (std::is_unsigned_v<T>) {
          return std::stoull(std::string(str));
        } else if constexpr (std::is_same_v<std::string, T>) {
          return std::string(str);
        } else {
          return T::fromString(str);
        }
      }

      static constexpr size_t arity = 1u;
    };

    template <> struct Traits<bool> {
      static bool fromString(std::string_view str) {
        return true;
      }

      static constexpr size_t arity = 0u;
    };

    struct Option {
      explicit Option(std::string_view name) : name_(name) {}

      virtual ~Option() = default;

      std::string_view name() const {
        return name_;
      }

      virtual void set(std::string_view val) = 0;
      virtual size_t arity() const = 0;

    private:
      std::string name_;
    };

    template <class T> struct TypedOption : public Option {
      explicit TypedOption(std::string_view name) : Option(name) {}

      explicit operator bool() const {
        return static_cast<bool>(value_);
      }

      const T& operator*() const {
        return *value_;
      }

      void set(std::string_view val) override {
        value_ = Traits<T>::fromString(val);
      }

      size_t arity() const override {
        return Traits<T>::arity;
      }

    private:
      std::optional<T> value_;
    };

    template <size_t N, class F> struct LambdaOption : public Option {
      LambdaOption(std::string_view name, F&& fun)
        : Option(name), fun_(std::move(fun)) {}

      void set(std::string_view val) override {
        if constexpr (N == 0) {
          fun_();
        } else {
          fun_(val);
        }
      }

      size_t arity() const override {
        return N;
      }

    private:
      F fun_;
    };
  } // namespace Detail

  template <class T> struct Option {
    explicit Option(std::shared_ptr<Detail::TypedOption<T>> detail)
      : detail_(std::move(detail)) {}

    std::string_view name() const {
      return detail_->name();
    }

    explicit operator bool() const {
      return static_cast<bool>(*detail_);
    }

    const T* operator->() const {
      return &**detail_;
    }

    const T& operator*() const {
      return **detail_;
    }

    bool operator==(const T& other) const {
      if (not*detail_) {
        return false;
      }

      if constexpr (std::is_floating_point_v<T>) {
        return Common::eql(**detail_, other);
      } else {
        return **detail_ == other;
      }
    }

    bool operator!=(const T& other) const {
      return not operator==(other);
    }

  private:
    std::shared_ptr<Detail::TypedOption<T>> detail_;
  };
} // namespace Common::OptionParser
