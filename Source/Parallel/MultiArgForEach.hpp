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
#include <Common/IndexList.hpp>
#include <thread>
#include <atomic>
#include <numeric>

namespace Common
{
namespace Parallel
{
namespace Detail
{

template <class F, class... Args>
class MultiArgForEachContext
{
public:
    MultiArgForEachContext(F&& f, size_t threadAmount, Args&&... args)
        : data(std::forward<Args>(args)...),
          dataSize(std::get<0>(data).size()),
          functor(std::forward<F>(f)),
          threads(threadAmount)
    {
        assert(totalDataSize(typename GenerateIndexList<sizeof...(Args)>::type()) == sizeof...(Args) * dataSize);

        for (auto& thread : threads)
            thread = std::thread(&MultiArgForEachContext::threadProcedure, this);
    }
    MultiArgForEachContext(const MultiArgForEachContext&) = delete;
    MultiArgForEachContext(MultiArgForEachContext&&) = delete;
    ~MultiArgForEachContext()
    {
        std::for_each(std::begin(threads), std::end(threads), std::mem_fn(&std::thread::join));
    }

    MultiArgForEachContext& operator = (const MultiArgForEachContext&) = delete;
    MultiArgForEachContext& operator = (MultiArgForEachContext&&) = delete;

    void wait()
    {
        std::for_each(std::begin(threads), std::end(threads), std::mem_fn(&std::thread::join));
        threads.clear();
    }

private:
    void threadProcedure()
    {
        for (size_t i = currentIndex.fetch_add(1); i < dataSize; i = currentIndex.fetch_add(1))
            callFunctor(i, typename GenerateIndexList<sizeof...(Args)>::type());
    }

    template <int... T>
    void callFunctor(size_t index, IndexList<T...>) const
    {
        functor(std::get<T>(data)[index]...);
    }

    template <int... T>
    size_t totalDataSize(IndexList<T...>) const
    {
        size_t results[sizeof...(Args)] = { std::get<T>(data).size()... };
        return std::accumulate(std::begin(results), std::end(results), size_t(0));
    }

    const std::tuple<const Args&...> data;
    const size_t dataSize;
    const F functor;

    std::atomic<size_t> currentIndex{0};
    std::vector<std::thread> threads;
};

}

template <class F, class... Args>
auto multiArgForEach(F&& f, size_t threadAmount, Args&&... args)
{
    using Context = Detail::MultiArgForEachContext<F, Args...>;
    return std::make_unique<Context>(std::forward<F>(f), threadAmount, std::forward<Args>(args)...);
}

template <class F, class... Args>
auto multiArgForEach(F&& f, Args&&... args)
{
    using Context = Detail::MultiArgForEachContext<F, Args...>;
    return std::make_unique<Context>(std::forward<F>(f), std::thread::hardware_concurrency(), std::forward<Args>(args)...);
}

}
}
