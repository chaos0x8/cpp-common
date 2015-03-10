#pragma once

#include <Generated/CacheLineSize.hpp>
#include <Parallel/Detail/Align.hpp>
#include <thread>
#include <atomic>
#include <algorithm>

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
        std::for_each(std::begin(threads), std::end(threads), std::mem_fn(&std::thread::join));
    }

    ComputeContext<T, F, R>& operator = (const ComputeContext<T, F, R>&) = delete;
    ComputeContext<T, F, R>& operator = (ComputeContext<T, F, R>&&) = delete;

    void wait()
    {
        std::for_each(std::begin(threads), std::end(threads), std::mem_fn(&std::thread::join));
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

