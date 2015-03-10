#pragma once

#include <Parallel/ThreadSafeQueue.hpp>
#include <boost/range/algorithm_ext/erase.hpp>
#include <thread>
#include <future>
#include <functional>
#include <vector>
#include <memory>

namespace Common
{
namespace Parallel
{

using ThreadPoolTask = std::function<void ()>;

class ThreadPool
{
public:
    ThreadPool(size_t threadAmount = std::thread::hardware_concurrency());
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ~ThreadPool();

    ThreadPool& operator = (const ThreadPool&) = delete;
    ThreadPool& operator = (ThreadPool&&) = delete;

    void queueTask(ThreadPoolTask task);
    void queueTasks(std::vector<ThreadPoolTask> tasks);

    template <class T, class A1, class... Args>
    void queueTask(T&& obj, A1&& arg1, Args&&... args)
    {
        q.push(std::bind(std::forward<T>(obj), std::forward<A1>(arg1), std::forward<Args>(args)...));
    }

    template <class T, class... Args>
    auto queueFuture(T&& obj, Args&&... args) -> std::future<decltype(obj(args...))>
    {
        using result_type = decltype(obj(std::forward<Args>(args)...));
        using packaged_task = std::packaged_task<result_type ()>;
        std::shared_ptr<packaged_task> task = std::make_shared<packaged_task>(
            std::bind(std::forward<T>(obj), std::forward<Args>(args)...));
        queueTask([task] { task->operator()(); });
        return task->get_future();
    }

private:
    void threadProcedure();

    ThreadSafeQueue<ThreadPoolTask> q;
    std::vector<std::thread> threads;
};

}
}
