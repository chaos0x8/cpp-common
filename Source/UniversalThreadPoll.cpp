#include <UniversalThreadPoll.hpp>
#include <algorithm>

namespace Common
{

UniversalThreadPoll::UniversalThreadPoll(size_t threadCount)
    : threads(threadCount ? threadCount : 1)
{
    finishedTaskCount = 0;

    for (auto& t : threads)
        t = std::thread(&UniversalThreadPoll::threadProc, this);
}

UniversalThreadPoll::~UniversalThreadPoll()
{
    deactivatePoll();
    std::for_each(std::begin(threads), std::end(threads), std::mem_fn(&std::thread::join));
}

void UniversalThreadPoll::queueTask(task_type task)
{
    std::unique_lock<std::mutex> lock(taskLock);
    addedTaskCount++;
    this->tasks.push_back(task);
    resumeThread.notify_one();
}

void UniversalThreadPoll::queueTasks(const std::vector<task_type>& tasks)
{
    std::unique_lock<std::mutex> lock(taskLock);
    addedTaskCount += tasks.size();
    this->tasks.insert(std::end(this->tasks), std::begin(tasks), std::end(tasks));
    resumeThread.notify_all();
}

void UniversalThreadPoll::synchronize()
{
    std::unique_lock<std::mutex> lock(taskLock);
    actionFinished.wait(lock, [this] { return addedTaskCount == finishedTaskCount; });
}

void UniversalThreadPoll::threadProc(UniversalThreadPoll* _this)
{
    while (task_type task = _this->aquireTask())
    {
        task();

        _this->finishedTaskCount++;
        _this->actionFinished.notify_one();
    }
}

void UniversalThreadPoll::deactivatePoll()
{
    std::unique_lock<std::mutex> lock(taskLock);
    activeFlag = false;
    resumeThread.notify_all();
}

UniversalThreadPoll::task_type UniversalThreadPoll::aquireTask()
{
    std::unique_lock<std::mutex> lock(taskLock);
    resumeThread.wait(lock, [this] { return tasks.size() || !activeFlag; });

    if (tasks.empty())
        return task_type();

    task_type task = tasks.front();
    tasks.pop_front();
    return task;
}

}
