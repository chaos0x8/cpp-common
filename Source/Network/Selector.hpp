/*!
 *  \author <https://github.com/chaos0x8>
 *  \copyright
 *  Copyright (c) 2015 - 2016, <https://github.com/chaos0x8>
 *
 *  \copyright
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  \copyright
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#pragma once

#include <Network/Pipe.hpp>
#include <atomic>
#include <functional>
#include <map>
#include <mutex>
#include <thread>

namespace Common::Network {
  using SelectorCallback = std::function<void(NativeHandler)>;

  class Selector {
  public:
    Selector();
    Selector(const Selector&) = delete;
    ~Selector();

    Selector& operator=(const Selector&) = delete;

    void stop();
    void wait();

    template <class T, class F, class... Args>
    std::enable_if_t<sizeof...(Args) != 0, void> add(
      const T& t, F&& f, Args&&... args) {
      const auto fd = getNativeHandler(t);
      addPriv(fd, std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    }

    template <class T, class F, class... Args>
    std::enable_if_t<sizeof...(Args) == 0, void> add(
      const T& t, F&& f, Args&&... args) {
      const auto fd = getNativeHandler(t);
      addPriv(fd, std::forward<F>(f));
    }

    template <class T> void remove(const T& t) {
      remove(getNativeHandler(t));
    }

  private:
    template <class T> static NativeHandler getNativeHandler(const T& t);

    void clear();

    void addPriv(NativeHandler fd, SelectorCallback);
    void remove(NativeHandler fd);
    void threadProcedure();
    void internalHandler(NativeHandler);

    std::atomic<bool> keepAlive{true};
    std::mutex itemsMutex;
    Pipe internalCommunication;
    std::thread selectorThread;
    std::map<NativeHandler, SelectorCallback> items;
  };

  template <class T>
  inline NativeHandler Selector::getNativeHandler(const T& t) {
    return t.getNativeHandler();
  }

  template <>
  inline NativeHandler Selector::getNativeHandler<Pipe>(const Pipe& t) {
    return t.getNativeHandler()[0];
  }
} // namespace Common::Network
