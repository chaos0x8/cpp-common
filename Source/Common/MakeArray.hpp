#pragma once

#include <array>

namespace Common {
  template <class T, class... Args>
  std::array<T, sizeof...(Args)> makeArray(Args&&... args) {
    return std::array<T, sizeof...(Args)>({std::forward<Args>(args)...});
  }
}
