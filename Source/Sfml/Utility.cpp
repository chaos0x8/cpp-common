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

#include <Sfml/Utility.hpp>
#include <vector>
#include <cassert>
#include <tuple>

namespace Common
{
namespace Sfml
{
namespace Detail
{

bool vmLess(const sf::VideoMode& left, const sf::VideoMode& right)
{
    return std::tie(left.bitsPerPixel, left.width, left.height)
        < std::tie(right.bitsPerPixel, right.width, right.height);
}

}

sf::VideoMode getFullScreenVideoMode()
{
    std::vector<sf::VideoMode> vm = sf::VideoMode::getFullscreenModes();
    assert(vm.size());
    std::sort(vm.begin(), vm.end(), &Detail::vmLess);
    return vm.back();
}

std::unique_ptr<sf::Window> createDefaultWindow(const std::string& title)
{
    #ifdef NDEBUG
    const sf::VideoMode vm = Common::Sfml::getFullScreenVideoMode();
    const auto style = sf::Style::Fullscreen;
    #else
    const sf::VideoMode vm = sf::VideoMode(1280, 720);
    const auto style = sf::Style::Default;
    #endif

    sf::ContextSettings settings = {};
    settings.antialiasingLevel = 4;

    std::unique_ptr<sf::Window> window(new sf::Window(vm, title, style, settings));
    window->setVerticalSyncEnabled(true);

    return window;
}

std::pair<GLfloat, GLfloat> getWsadSpeedFactor()
{
    std::pair<GLfloat, GLfloat> result = { 0.f, 0.f };

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        std::get<0>(result) = 1.0f;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        std::get<0>(result) = -1.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        std::get<1>(result) = 1.0f;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        std::get<1>(result) = -1.0f;

    return result;
}

std::pair<GLfloat, GLfloat> getArrowSpeedFactor()
{
    std::pair<GLfloat, GLfloat> result = { 0.f, 0.f };

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        std::get<0>(result) = 1.0f;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        std::get<0>(result) = -1.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        std::get<1>(result) = 1.0f;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        std::get<1>(result) = -1.0f;

    return result;
}

}
}
