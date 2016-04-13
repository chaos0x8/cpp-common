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

#include <GL/Shader.hpp>
#include <GL/Detail/CheckLog.hpp>
#include <Common/FileUtility.hpp>

namespace Common
{
namespace GL
{
namespace Detail
{

GLuint ShaderAllocator::operator() (const std::string& fileName, GLenum shaderType) const
{
    const std::string shaderCode = Common::readFile(fileName);
    const char* codePtr = shaderCode.c_str();

    constexpr GLsizei SHADER_COUNT = 1;
    const GLuint shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, SHADER_COUNT, &codePtr, 0);
    glCompileShader(shaderId);

    Detail::checkLog(shaderId, GL_COMPILE_STATUS, glGetShaderInfoLog);

    return shaderId;
}

void ShaderDeleter::operator() (GLuint shaderId) const noexcept
{
    glDeleteShader(shaderId);
}

}
}
}
