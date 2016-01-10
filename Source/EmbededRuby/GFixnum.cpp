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

#include <EmbededRuby/GFixnum.hpp>
#include <ruby.h>

namespace Common
{
namespace EmbededRuby
{

GFixnum::GFixnum(const std::string& name, long value)
    : _name(name)
{
    VALUE v = LONG2FIX(value);
    rb_gv_set(_name.c_str(), v);
}

GFixnum& GFixnum::operator = (long value)
{
    VALUE v = LONG2FIX(value);
    rb_gv_set(_name.c_str(), v);

    return *this;
}

GFixnum::operator long() const
{
    VALUE v = rb_gv_get(_name.c_str());
    return FIX2LONG(v);
}

}
}
