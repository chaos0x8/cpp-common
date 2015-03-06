#pragma once

#include <Generated/CacheLineSize.hpp>
#include <thread>
#include <atomic>
#include <algorithm>

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
        : t(std::forward<T>(t)),
          f(std::forward<F>(f)),
          threads(threadAmount)
    {
        for (auto& thread : threads)
            thread = std::thread(&ForEachContext<T, F>::threadProcedure, this);
    }
    ForEachContext(const ForEachContext&) = delete;
    ForEachContext(ForEachContext&&) = delete;
    ~ForEachContext()
    {
        std::for_each(std::begin(threads), std::end(threads), std::mem_fn(&std::thread::join));
    }

    ForEachContext<T, F>& operator = (const ForEachContext&) = delete;
    ForEachContext<T, F>& operator = (ForEachContext&&) = delete;

    void wait()
    {
        std::for_each(std::begin(threads), std::end(threads), std::mem_fn(&std::thread::join));
        threads.clear();
    }

private:
    void threadProcedure()
    {
        const size_t dataSize = t.size();
        for (size_t i = currentIndex.fetch_add(PROGRESS); i < dataSize; i = currentIndex.fetch_add(PROGRESS))
        {
            const size_t partEnd = std::min(dataSize, i + PROGRESS);
            for (size_t k = i; k < partEnd; ++k)
                f(t[k]);
        }
    }

    T t;
    F f;

    static constexpr size_t PROGRESS = (CACHE_LINE_SIZE*2ul) / sizeof(typename std::decay<T>::type::value_type);
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

