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

#include <GL/Detail/CheckLog.hpp>
#include <vector>
#include <stdexcept>

namespace Common
{
namespace GL
{
namespace Detail
{

void checkLog(const GLuint& id, GLenum eventType, std::function<void (GLuint, GLsizei, GLsizei*, GLchar*)> getInforLog)
{
    GLint result = 0;
    GLsizei infoLogLength = 0;
    glGetShaderiv(id, eventType, &result);
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);

    if (infoLogLength)
    {
        std::vector<char> infoLog(infoLogLength);
        getInforLog(id, infoLogLength, 0, infoLog.data());
        throw std::runtime_error(std::string(infoLog.data(), infoLog.size()));
    }
}

}
}
}
