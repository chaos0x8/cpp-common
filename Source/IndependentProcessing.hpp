#pragma once
#include <iterator>
#include <memory>
#include <thread>
#include <vector>
#include <boost/optional.hpp>

namespace Common
{
namespace Detail
{

template <typename T, typename F>
class IndependentProcessorBase
{
public:
    IndependentProcessorBase(T data, F functor, size_t threadAmount)
        : data(data),
          functor(functor),
          threads(threadAmount),
          partSize(data.size() / threadAmount + (data.size() % threadAmount == 0 ? 0 : 1))
    {
    }
    ~IndependentProcessorBase()
    {
        join();
    }

    void join()
    {
        for (std::thread& t : threads)
            t.join();
        threads.clear();
    }

protected:
    T data;
    F functor;
    std::vector<std::thread> threads;
    const size_t partSize;
};

template <typename R>
class IndependentProcessorData
{
public:
    IndependentProcessorData(size_t threadAmount)
        : results(threadAmount)
    {
    }

protected:
    std::vector<R> get() const
    {
        std::vector<R> res;

        for (const std::vector<R>& x : results)
            res.insert(std::end(res), std::begin(x), std::end(x));

        return res;
    }

    std::vector<std::vector<R>> results;
};

template <typename T, typename F, typename R>
class IndependentProcessor : public IndependentProcessorBase<T, F>,
                             public IndependentProcessorData<R>
{
public:
    IndependentProcessor(T data, F functor, size_t threadAmount)
        : IndependentProcessorBase<T, F>(data, functor, threadAmount),
          IndependentProcessorData<R>(threadAmount)
    {
        for (size_t threadId = 0; threadId < this->threads.size(); ++threadId)
            this->threads[threadId] = std::thread([this, threadId]()
            {
                for (size_t i = this->partSize * threadId; i < this->partSize * (threadId + 1) && i < this->data.size(); ++i)
                   this->results[threadId].push_back(this->functor(this->data[i]));
            });
    }

    std::vector<R> get()
    {
        this->join();
        return this->IndependentProcessorData<R>::get();
    }
};

template <typename T, typename F, typename R>
class IndependentProcessor<T, F, boost::optional<R>> : public IndependentProcessorBase<T, F>,
                                                       public IndependentProcessorData<R>
{
public:
    IndependentProcessor(T data, F functor, size_t threadAmount)
        : IndependentProcessorBase<T, F>(data, functor, threadAmount),
          IndependentProcessorData<R>(threadAmount)
    {
        for (size_t threadId = 0; threadId < this->threads.size(); ++threadId)
            this->threads[threadId] = std::thread([this, threadId]()
            {
                for (size_t i = this->partSize * threadId; i < this->partSize * (threadId + 1) && i < this->data.size(); ++i)
                {
                    auto res = this->functor(this->data[i]);
                    if (res.is_initialized())
                        this->results[threadId].push_back(res.get());
                }
            });
    }

    std::vector<R> get()
    {
        this->join();
        return this->IndependentProcessorData<R>::get();
    }
};

template <typename T, typename F>
class IndependentProcessor<T, F, void> : public IndependentProcessorBase<T, F>
{
public:
    IndependentProcessor(T data, F functor, size_t threadAmount)
        : IndependentProcessorBase<T, F>(data, functor, threadAmount)
    {
        for (size_t threadId = 0; threadId < this->threads.size(); ++threadId)
            this->threads[threadId] = std::thread([this, threadId]()
            {
                for (size_t i = this->partSize * threadId; i < this->partSize * (threadId + 1) && i < this->data.size(); ++i)
                    this->functor(this->data[i]);
            });
    }
};

}

template <typename T, typename F>
auto runIndependentProcessing(T&& data, F&& functor, size_t threadAmount = std::thread::hardware_concurrency())
{
    using ReturnType = decltype(functor(data[0]));
    return std::unique_ptr<Detail::IndependentProcessor<T, F, ReturnType>>(new Detail::IndependentProcessor<T, F, ReturnType>(data, functor, threadAmount));
}

}
