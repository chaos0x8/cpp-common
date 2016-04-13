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

#include <curses.h>
#include <string>
#include <functional>

namespace Common
{
namespace NCurses
{

class Window
{
public:
    Window();
    Window(const Window&) = delete;
    Window(int height, int width, int y, int x);
    ~Window();

    void clear();
    void refresh();
    bool refresh(std::function<void ()>);
    void move(int y, int x);
    void print(const std::string&, int y, int x);
    void print(const std::string&);
    void print(chtype, int y, int x);
    void print(chtype);
    int read();
    int read(int y, int x);

    int width() const { return _width; }
    int height() const { return _height; }

    Window& operator = (const Window&) = delete;

private:
    int _width;
    int _height;
    WINDOW* win{nullptr};
};

}
}
