/*!
 *  \author <https://github.com/chaos0x8>
 *  \copyright
 *  Copyright (c) 2016, <https://github.com/chaos0x8>
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

#include <functional>

namespace Common
{
namespace ObjectSpace
{

template <class T>
struct Accessor
{
    using value_type = T;

    explicit Accessor(const std::function<T ()>& reader,
                      const std::function<void (const T&)>& writer)
        : reader(reader), writer(writer) { }
    Accessor(const Accessor<T>&) = delete;
    Accessor(Accessor<T>&&) = delete;

    Accessor<T>& operator = (const Accessor<T>& other)
    {
        writer(other.reader());
        return *this;
    }
    Accessor<T>& operator = (const T& other)
    {
        writer(other);
        return *this;
    }
    Accessor<T>& operator = (Accessor<T>&&) = delete;

    operator T() const
        { return reader(); }

    bool operator == (const Accessor<T>& other) const
        { return reader() == other.reader(); }
    bool operator == (const T& other) const
        { return reader() == other; }
    bool operator != (const Accessor<T>& other) const
        { return reader() != other.reader(); }
    bool operator != (const T& other) const
        { return reader() != other; }

private:
    std::function<T ()> reader;
    std::function<void (const T&)> writer;
};

template <class T>
struct Reader
{
    using value_type = T;

    explicit Reader(const std::function<T ()>& reader)
        : reader(reader) { }
    Reader(const Reader<T>&) = delete;
    Reader(Reader<T>&&) = delete;

    Reader<T>& operator = (const Reader<T>&) = delete;
    Reader<T>& operator = (Reader<T>&&) = delete;

    operator T() const
        { return reader(); }

    bool operator == (const Reader<T>& other) const
        { return reader() == other.reader(); }
    bool operator == (const T& other) const
        { return reader() == other; }
    bool operator != (const Reader<T>& other) const
        { return reader() != other.reader(); }
    bool operator != (const T& other) const
        { return reader() != other; }

private:
    std::function<T ()> reader;
};

template <class T>
struct Writer
{
    using value_type = T;

    explicit Writer(const std::function<void (const T&)>& writer)
        : writer(writer) { }
    Writer(const Writer<T>&) = delete;
    Writer(Writer<T>&&) = delete;

    Writer<T>& operator = (const Writer<T>& other) = delete;
    Writer<T>& operator = (const T& other)
    {
        writer(other);
        return *this;
    }
    Writer<T>& operator = (Writer<T>&&) = delete;

    bool operator == (const Writer<T>&) const = delete;
    bool operator != (const Writer<T>&) const = delete;

private:
    std::function<void (const T&)> writer;
};

}
}
