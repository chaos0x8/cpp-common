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
    for (size_t i = 0; i < threads.size(); ++i)
        q.push(ThreadPoolTask());

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
