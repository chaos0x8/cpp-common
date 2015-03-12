/*!
    \author <https://github.com/chaos0x8>
    \copyright
    Copyright (c) 2015, <https://github.com/chaos0x8>

    \copyright
    Permission to use, copy, modify, and/or distribute this software for any
    purpose with or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    \copyright
    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <Parallel/ThreadPool.hpp>

namespace Common
{
namespace Parallel
{

ThreadPool::ThreadPool(size_t threadAmount)
    : threads(threadAmount)
{
    for (auto& t : threads)
        t = std::thread(std::mem_fn(&ThreadPool::threadProcedure), this);
}

ThreadPool::~ThreadPool()
{
    std::vector<ThreadPoolTask> emptyTasks(threads.size(), ThreadPoolTask());
    q.push(std::begin(emptyTasks), std::end(emptyTasks));

    std::for_each(std::begin(threads), std::end(threads), std::mem_fn(&std::thread::join));

    while (q.size())
        if (ThreadPoolTask t = q.pop())
            t();
}

void ThreadPool::queueTask(ThreadPoolTask task)
{
    if (task)
        q.push(std::move(task));
}

void ThreadPool::queueTasks(std::vector<ThreadPoolTask> tasks)
{
    boost::range::remove_erase_if(tasks, [] (const ThreadPoolTask& task) { return !static_cast<bool>(task); });
    q.push(std::begin(tasks), std::end(tasks));
}

void ThreadPool::threadProcedure()
{
    while (ThreadPoolTask t = q.pop())
        t();
}

}
}
