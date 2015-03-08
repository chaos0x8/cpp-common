#pragma once

#include <Parallel/ThreadSafeQueue.hpp>
#include <boost/range/algorithm_ext/erase.hpp>
#include <thread>
#include <functional>
#include <vector>

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

private:
    void threadProcedure();

    ThreadSafeQueue<ThreadPoolTask> q;
    std::vector<std::thread> threads;
};

}
}
