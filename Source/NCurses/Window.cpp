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

#include <NCurses/Window.hpp>
#include <iostream>

namespace Common
{
namespace NCurses
{

Window::Window()
{
    getmaxyx(stdscr, _height, _width);
    win = newwin(_height, _width, 0, 0);
}

Window::Window(int height, int width, int y, int x)
    : _height(height),
      _width(width)
{
    win = newwin(_height, _width, y, x);
}

Window::~Window()
{
    delwin(win);
}

void Window::clear()
{
    wclear(win);
}

void Window::refresh()
{
    wrefresh(win);
}

bool Window::refresh(std::function<void ()> operation)
{
    try
    {
        operation();
    }
    catch (const std::exception&)
    {
        wrefresh(win);
        return false;
    }

    wrefresh(win);
    return true;
}

void Window::move(int y, int x)
{
    wmove(win, y, x);
}

void Window::print(const std::string& text)
{
    waddstr(win, text.c_str());
}

void Window::print(const std::string& text, int y, int x)
{
    mvwaddstr(win, y, x, text.c_str());
}

void Window::print(chtype ch, int y, int x)
{
    mvwaddch(win, y, x, ch);
}

void Window::print(chtype ch)
{
    waddch(win, ch);
}

int Window::read()
{
    return wgetch(win);
}

int Window::read(int y, int x)
{
    return mvwgetch(win, y, x);
}

}
}
