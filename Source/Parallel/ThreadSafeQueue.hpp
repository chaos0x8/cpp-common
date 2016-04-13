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
