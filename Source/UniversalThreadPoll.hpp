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

class UniversalThreadPoll
{
public:
    using task_type = std::function<void ()>;

    UniversalThreadPoll(size_t threadCount = std::thread::hardware_concurrency());
    UniversalThreadPoll(const UniversalThreadPoll&) = delete;
    UniversalThreadPoll(UniversalThreadPoll&&) = delete;
    ~UniversalThreadPoll();

    UniversalThreadPoll& operator = (const UniversalThreadPoll&) = delete;
    UniversalThreadPoll& operator = (UniversalThreadPoll&&) = delete;

    void queueTask(task_type task);
    void queueTasks(const std::vector<task_type>& tasks);
    void synchronize();

private:
    static void threadProc(UniversalThreadPoll* _this);

    void deactivatePoll();
    task_type aquireTask();

    bool activeFlag = true;
    size_t addedTaskCount = 0u;
    std::atomic<size_t> finishedTaskCount;

    std::mutex taskLock;
    std::condition_variable resumeThread;
    std::condition_variable actionFinished;
    std::vector<std::thread> threads;
    std::deque<task_type> tasks;
};

}
