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

template <class T, class F, class R>
class ComputeContext
{
    using Allocator = typename std::decay<T>::type::allocator_type:: template rebind<R>::other;

public:
    ComputeContext(T&& t, F&& f, size_t threadAmount)
        : data(std::forward<T>(t)),
          functor(std::forward<F>(f)),
          threads(threadAmount),
          result(data.size())
    {
        const size_t unaligned = Align<R>::unaligned(result.data());
        currentIndex.fetch_add(unaligned);

        for (auto& thread : threads)
            thread = std::thread(&ComputeContext<T, F, R>::threadProcedure, this);

        const size_t dataSize = data.size();
        for (size_t k = 0; k < unaligned && k < dataSize; ++k)
            result[k] = functor(data[k]);
    }
    ComputeContext(const ComputeContext<T, F, R>&) = delete;
    ComputeContext(ComputeContext<T, F, R>&&) = delete;
    ~ComputeContext()
    {
      for (auto& t : threads)
        t.join();
    }

    ComputeContext<T, F, R>& operator = (const ComputeContext<T, F, R>&) = delete;
    ComputeContext<T, F, R>& operator = (ComputeContext<T, F, R>&&) = delete;

    void wait()
    {
      for (auto& t : threads)
        t.join();
      threads.clear();
    }

    std::vector<R, Allocator> get()
    {
        wait();
        return std::move(result);
    }

private:
    void threadProcedure()
    {
        const size_t dataSize = data.size();
        for (size_t i = currentIndex.fetch_add(PROGRESS); i < dataSize; i = currentIndex.fetch_add(PROGRESS))
        {
            const size_t partEnd = std::min(dataSize, i + PROGRESS);
            for (size_t k = i; k < partEnd; ++k)
                result[k] = functor(data[k]);
        }
    }

    T data;
    F functor;

    static constexpr size_t PROGRESS = Align<R>::progress;
    std::atomic<size_t> currentIndex{0};
    std::vector<std::thread> threads;
    std::vector<R, Allocator> result;
};

}

template <class T, class F>
auto compute(T&& t, F&& f, size_t threadAmount = std::thread::hardware_concurrency())
{
    using R = decltype(f(t[0]));
    using Context = Detail::ComputeContext<T, F, R>;
    return std::unique_ptr<Context>(new Context(std::forward<T>(t), std::forward<F>(f), threadAmount));
}

}
}
