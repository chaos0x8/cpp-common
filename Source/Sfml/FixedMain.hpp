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

#pragma once

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <memory>
#include <chrono>

#define SFML_FIXED_MAIN(ApplicationClass) \
    int main(int argc, char** argv) \
    { \
        using namespace std::chrono; \
        \
        std::unique_ptr<ApplicationClass> app; \
        { \
            sf::Context context; \
            glewExperimental = GL_TRUE; \
            glewInit(); \
            \
            app = std::unique_ptr<ApplicationClass>(new ApplicationClass(argc, argv)); \
            assert(app->window != nullptr); \
        } \
        \
        time_point<high_resolution_clock> start = high_resolution_clock::now(); \
        time_point<high_resolution_clock> previousFrame = start; \
        while (true) \
        { \
            sf::Event windowEvent; \
            while (app->window->pollEvent(windowEvent)) \
                app->handleEvent(windowEvent); \
            \
            if (!app->window->isOpen()) \
                break; \
            \
            time_point<high_resolution_clock> current = high_resolution_clock::now(); \
            app->render( \
                duration_cast<milliseconds>(current - previousFrame), \
                duration_cast<milliseconds>(current - start)); \
            previousFrame = current; \
            \
            app->window->display(); \
        } \
        \
        return 0; \
    }
