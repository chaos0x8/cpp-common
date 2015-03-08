#pragma once

#include <deque>
#include <mutex>
#include <condition_variable>

namespace Common
{
namespace Parallel
{

template <class T>
class ThreadSafeQueue
{
public:
    void push(T value)
    {
        std::unique_lock<std::mutex> lock(m);
        q.push_back(std::move(value));
        con.notify_one();
    }

    template <class It>
    void push(It begin, It end)
    {
        std::unique_lock<std::mutex> lock(m);
        q.insert(std::end(q), begin, end);
        con.notify_all();
    }

    T pop()
    {
        std::unique_lock<std::mutex> lock(m);
        con.wait(lock, [this] () -> bool { return q.size(); });

        T value{};
        std::swap(value, q.front());
        q.pop_front();

        return value;
    }

    size_t size() const
    {
        std::unique_lock<std::mutex> lock(m);
        return q.size();
    }

private:
    mutable std::mutex m;
    std::condition_variable con;
    std::deque<T> q;
};

}
}
