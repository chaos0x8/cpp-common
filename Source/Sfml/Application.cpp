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

#include <Sfml/Application.hpp>

namespace Common
{
namespace Sfml
{

Application::Application(int argc, char** argv)
{
    for (int i = 1; i < argc; ++i)
        this->args.emplace_back(argv[i]);
}

void Application::handleEvent(const sf::Event& event)
{
  switch (event.type)
  {
    case sf::Event::Closed:
      window->close();
    case sf::Event::KeyPressed:
      keyPressed(event.key);
      break;
    case sf::Event::MouseWheelMoved:
      mouseWheelDelta = event.mouseWheel.delta;
      break;
    case sf::Event::Resized:
      resized(event.size);
      break;
    default:
      break;
  }
}

void Application::keyPressed(const sf::Event::KeyEvent& keyEvent)
{
  switch (keyEvent.code)
  {
    case sf::Keyboard::Key::Escape:
      window->close();
      break;
    case sf::Keyboard::Key::F4:
      if (keyEvent.alt)
        window->close();
      break;
    default:
      break;
  }
}

void Application::resized(const sf::Event::SizeEvent& sizeEvent)
{
    this->windowSize.x = static_cast<decltype(this->windowSize.x)>(sizeEvent.width);
    this->windowSize.y = static_cast<decltype(this->windowSize.y)>(sizeEvent.height);
    glViewport(0, 0, this->windowSize.x, this->windowSize.y);
}

glm::vec2 Application::lockCursor() const
{
    const sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
    sf::Mouse::setPosition(sf::Vector2i(windowSize.x / 2, windowSize.y / 2), *window);

    return glm::vec2((windowSize.x/2) - static_cast<GLfloat>(mousePos.x),
                     (windowSize.y/2) - static_cast<GLfloat>(mousePos.y));
}

}
}
