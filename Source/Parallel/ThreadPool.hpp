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
