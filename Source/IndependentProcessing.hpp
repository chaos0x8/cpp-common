#pragma once
#include <iterator>
#include <memory>
#include <thread>
#include <vector>
#include <boost/optional.hpp>
#include <atomic>

namespace Common
{
namespace Detail
{

template <class T, class F, template <class> class Allocator>
class IndependentProcessorBase
{
public:
    IndependentProcessorBase(T data, F functor, size_t threadAmount)
        : data(data),
          functor(functor),
          threads(threadAmount),
          partSize(data.size() / threadAmount + (data.size() % threadAmount == 0 ? 0 : 1))
    {
        dataIndex = 0;
    }

    inline void join()
    {
        for (std::thread& t : threads)
            t.join();
        threads.clear();
    }

protected:
    T data;
    F functor;
    std::vector<std::thread, Allocator<std::thread>> threads;
    const size_t partSize;

    std::atomic<size_t> dataIndex;
};

template <class R, template <class> class Allocator>
class IndependentProcessorData
{
public:
    IndependentProcessorData(size_t threadAmount)
        : results(threadAmount)
    {
    }

protected:
    inline std::vector<R, Allocator<R>> get() const
    {
        const size_t totalSize = std::accumulate(std::begin(results), std::end(results), size_t(),
        [](size_t y, const std::vector<R, Allocator<R>>& x) -> size_t
        {
            return y + x.size();
        });

        std::vector<R, Allocator<R>> res;
        res.reserve(totalSize);

        for (auto& x : results)
            for (auto& y : x)
                res.emplace_back(std::move(y));

        return res;
    }

    using internal_value_type = std::vector<R, Allocator<R>>;

    std::vector<internal_value_type, Allocator<internal_value_type>> results;
};

template <class T, class F, class R, template <class> class Allocator>
class IndependentProcessor : public IndependentProcessorBase<T, F, Allocator>,
                             public IndependentProcessorData<R, Allocator>
{
public:
    using self_type = IndependentProcessor<T, F, R, Allocator>;

    IndependentProcessor(T data, F functor, size_t threadAmount)
        : IndependentProcessorBase<T, F, Allocator>(data, functor, threadAmount),
          IndependentProcessorData<R, Allocator>(threadAmount)
    {
        for (size_t threadId = 0; threadId < this->threads.size(); ++threadId)
            this->threads[threadId] = std::thread(&self_type::threadProcedure, this, threadId);
    }
    ~IndependentProcessor()
    {
        this->join();
    }

    std::vector<R, Allocator<R>> get()
    {
        this->join();
        return this->IndependentProcessorData<R, Allocator>::get();
    }

private:
    inline void threadProcedure(size_t threadId)
    {
        std::vector<R, Allocator<R>> tmpResult;
        tmpResult.reserve(this->data.size() / this->threads.size());

        for (size_t i = this->dataIndex++; i < this->data.size(); i = this->dataIndex++)
            tmpResult.emplace_back(this->functor(this->data[i]));
        this->results[threadId] = std::move(tmpResult);
    }
};

template <class T, class F, class R, template <class> class Allocator>
class IndependentProcessor<T, F, boost::optional<R>, Allocator> : public IndependentProcessorBase<T, F, Allocator>,
                                                                  public IndependentProcessorData<R, Allocator>
{
public:
    using self_type = IndependentProcessor<T, F, boost::optional<R>, Allocator>;

    IndependentProcessor(T data, F functor, size_t threadAmount)
        : IndependentProcessorBase<T, F, Allocator>(data, functor, threadAmount),
          IndependentProcessorData<R, Allocator>(threadAmount)
    {
        for (size_t threadId = 0; threadId < this->threads.size(); ++threadId)
            this->threads[threadId] = std::thread(&self_type::threadProcedure, this, threadId);
    }
    ~IndependentProcessor()
    {
        this->join();
    }

    std::vector<R, Allocator<R>> get()
    {
        this->join();
        return this->IndependentProcessorData<R, Allocator>::get();
    }

private:
    inline void threadProcedure(size_t threadId)
    {
        std::vector<R, Allocator<R>> tmpResult;
        tmpResult.reserve(this->data.size() / this->threads.size());

        for (size_t i = this->dataIndex++; i < this->data.size(); i = this->dataIndex++)
        {
            auto res = this->functor(this->data[i]);
            if (res.is_initialized())
                tmpResult.emplace_back(std::move(res.get()));
        }
        this->results[threadId] = std::move(tmpResult);
    }
};

template <class T, class F, template <class> class Allocator>
class IndependentProcessor<T, F, void, Allocator> : public IndependentProcessorBase<T, F, Allocator>
{
public:
    using self_type = IndependentProcessor<T, F, void, Allocator>;

    IndependentProcessor(T data, F functor, size_t threadAmount)
        : IndependentProcessorBase<T, F, Allocator>(data, functor, threadAmount)
    {
        for (size_t threadId = 0; threadId < this->threads.size(); ++threadId)
            this->threads[threadId] = std::thread(&self_type::threadProcedure, this, threadId);
    }
    ~IndependentProcessor()
    {
        this->join();
    }

private:
    inline void threadProcedure(size_t threadId)
    {
        for (size_t i = this->dataIndex++; i < this->data.size(); i = this->dataIndex++)
            this->functor(this->data[i]);
    }
};

}

template <template<class> class Allocator = std::allocator, class T, class F>
auto runIndependentProcessing(T&& data, F&& functor, size_t threadAmount = std::thread::hardware_concurrency())
{
    using ReturnType = decltype(functor(data[0]));
    return std::unique_ptr<Detail::IndependentProcessor<T, F, ReturnType, Allocator>>(
        new Detail::IndependentProcessor<T, F, ReturnType, Allocator>(data, functor, threadAmount));
}

}
