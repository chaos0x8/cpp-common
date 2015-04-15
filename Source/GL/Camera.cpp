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

#include <GL/Camera.hpp>

namespace Common
{
namespace GL
{

glm::vec3 Camera::getDirection() const
{
    return glm::vec3(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle));
}

glm::vec3 Camera::getRight() const
{
    return glm::vec3(
        sin(horizontalAngle - 3.14f/2.0f),
        0.f,
        cos(horizontalAngle - 3.14f/2.0f));
}

glm::vec3 Camera::getUp() const
{
    return glm::cross(getRight(), getDirection());
}

glm::mat4 Camera::getView() const
{
    return glm::lookAt(eye, eye + getDirection(), getUp());
}

void Camera::move(GLfloat sideSpeed, GLfloat forwardSpeed, GLfloat elapsedTime)
{
    eye += getRight() * elapsedTime * sideSpeed;
    eye += getDirection() * elapsedTime * forwardSpeed;
}

}
}
