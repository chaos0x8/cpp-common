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

#include <Generated/CacheLineSize.hpp>
#include <Parallel/Detail/Align.hpp>
#include <thread>
#include <atomic>
#include <algorithm>
#include <vector>

namespace Common
{
namespace Parallel
{
namespace Detail
{

template <class T, class F>
class ForEachContext
{
public:
    ForEachContext(T&& t, F&& f, size_t threadAmount)
        : data(std::forward<T>(t)),
          functor(std::forward<F>(f)),
          threads(threadAmount)
    {
        const size_t unaligned = Align<typename std::decay<T>::type::value_type>::unaligned(data.data());
        currentIndex.fetch_add(unaligned);

        for (auto& thread : threads)
            thread = std::thread(&ForEachContext<T, F>::threadProcedure, this);

        const size_t dataSize = data.size();
        for (size_t k = 0; k < unaligned && k < dataSize; ++k)
            functor(data[k]);
    }
    ForEachContext(const ForEachContext&) = delete;
    ForEachContext(ForEachContext&&) = delete;
    ~ForEachContext()
    {
      for (auto& t : threads)
        t.join();
    }

    ForEachContext<T, F>& operator = (const ForEachContext&) = delete;
    ForEachContext<T, F>& operator = (ForEachContext&&) = delete;

    void wait()
    {
      for (auto& t : threads)
        t.join();
      threads.clear();
    }

private:
    void threadProcedure()
    {
        const size_t dataSize = data.size();
        for (size_t i = currentIndex.fetch_add(PROGRESS); i < dataSize; i = currentIndex.fetch_add(PROGRESS))
        {
            const size_t partEnd = std::min(dataSize, i + PROGRESS);
            for (size_t k = i; k < partEnd; ++k)
                functor(data[k]);
        }
    }

    T data;
    F functor;

    static constexpr size_t PROGRESS = Align<typename std::decay<T>::type::value_type>::progress;
    std::atomic<size_t> currentIndex{0};
    std::vector<std::thread> threads;
};

}

template <class T, class F>
auto forEach(T&& t, F&& f, size_t threadAmount = std::thread::hardware_concurrency())
{
    using Context = Detail::ForEachContext<T, F>;
    return std::unique_ptr<Context>(new Context(std::forward<T>(t), std::forward<F>(f), threadAmount));
}

}
}
