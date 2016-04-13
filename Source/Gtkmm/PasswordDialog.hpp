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

#include <gtkmm.h>
#include <glibmm.h>

namespace Common
{
namespace Gui
{

class PasswordDialog : public Gtk::Dialog
{
public:
    PasswordDialog(Gtk::Window&, const std::string& prompt, const std::string& title = "Password dialog");
    explicit PasswordDialog(const std::string& prompt, const std::string& title = "Password dialog");

    std::string execute();

protected:
    void construct();
    void initWidgets();
    void initLayout();

    Gtk::Label prompt;
    Gtk::Entry password;
    std::unique_ptr<Gtk::Button> ok;
    std::unique_ptr<Gtk::Button> cancel;
};

}
}
