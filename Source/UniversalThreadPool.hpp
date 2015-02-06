#pragma once

#include <vector>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <atomic>

namespace Common
{

class UniversalThreadPool
{
public:
    using task_type = std::function<void ()>;

    UniversalThreadPool(size_t threadCount = std::thread::hardware_concurrency());
    UniversalThreadPool(const UniversalThreadPool&) = delete;
    UniversalThreadPool(UniversalThreadPool&&) = delete;
    ~UniversalThreadPool();

    UniversalThreadPool& operator = (const UniversalThreadPool&) = delete;
    UniversalThreadPool& operator = (UniversalThreadPool&&) = delete;

    void queueTask(task_type task);
    void queueTasks(const std::vector<task_type>& tasks);
    void synchronize();
    bool isSynchronized() const;

private:
    static void threadProc(UniversalThreadPool* _this);

    void deactivatePool();
    task_type aquireTask();

    bool activeFlag = true;
    size_t addedTaskCount = 0u;
    std::atomic<size_t> finishedTaskCount;

    std::mutex taskLock;
    std::condition_variable resumeThread;
    std::vector<std::thread> threads;
    std::deque<task_type> tasks;
};

}
