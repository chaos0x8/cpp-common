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
