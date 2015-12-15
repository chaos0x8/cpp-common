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

#include <EmbededRuby/Evaluate.hpp>
#include <EmbededRuby/Exception.hpp>
#include <memory>
#include <ruby.h>

namespace Common
{
namespace EmbededRuby
{
namespace Detail
{

class RubyInit
{
public:
    RubyInit()
    {
        ruby_init();
        ruby_init_loadpath();
    }

    ~RubyInit()
    {
        ruby_cleanup(0);
    }
};

std::unique_ptr<RubyInit> rubyInit = nullptr;

std::string toString(VALUE value)
{
    auto str = rb_funcall(value, rb_intern("to_s"), 0);
    return std::string(RSTRING_PTR(str), RSTRING_LEN(str));
}

}

void init()
{
    if (Detail::rubyInit == nullptr)
        Detail::rubyInit = std::make_unique<Detail::RubyInit>();
}

bool execute(const char* code)
{
    int status = 0;
    rb_eval_string_protect(code, &status);
    return status == 0;
}

bool execute(const std::string& code)
{
    return execute(code.c_str());
}

std::string evaluate(const char* code)
{
    int status = 0;
    auto v = rb_eval_string_protect(code, &status);

    if (status != 0)
        throw EvaluateFailed();

    return Detail::toString(v);
}

std::string evaluate(const std::string& code)
{
    return evaluate(code.c_str());
}

}
}
