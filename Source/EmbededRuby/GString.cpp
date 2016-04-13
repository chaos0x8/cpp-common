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

#include <EmbededRuby/GString.hpp>
#include <EmbededRuby/Detail/Utility.hpp>
#include <ruby.h>

namespace Common
{
namespace EmbededRuby
{

GString::GString(const std::string& name, const std::string& value)
    : _name(name)
{
    VALUE v = rb_str_new_cstr(value.c_str());
    rb_gv_set(_name.c_str(), v);
}

GString& GString::operator = (const char* value)
{
    VALUE v = rb_str_new_cstr(value);
    rb_gv_set(_name.c_str(), v);

    return *this;
}

GString::operator std::string() const
{
    VALUE v = rb_gv_get(_name.c_str());
    return Detail::toString(v);
}

}
}
