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

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <memory>
#include <chrono>
#include <glm/glm.hpp>

namespace Common
{
namespace Sfml
{

class Application
{
public:
    Application(int argc, char** argv);
    virtual ~Application() noexcept = default;

    void handleEvent(const sf::Event& event);

    std::unique_ptr<sf::Window> window;

protected:
    virtual void keyPressed(const sf::Event::KeyEvent&);
    void resized(const sf::Event::SizeEvent&);

    glm::vec2 lockCursor() const;

    std::vector<std::string> args;
    sf::Vector2u windowSize = { };
};

}
}
