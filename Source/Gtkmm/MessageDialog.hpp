#pragma once

#include <gtkmm.h>

namespace Common
{
namespace Gui
{

Gtk::ResponseType messageDialog(
    const std::string& message,
    const std::string& title,
    Gtk::MessageType messageType = Gtk::MESSAGE_ERROR,
    Gtk::ButtonsType buttons = Gtk::BUTTONS_OK);
Gtk::ResponseType messageDialog(
    Gtk::Window& parent,
    const std::string& message,
    const std::string& title,
    Gtk::MessageType messageType = Gtk::MESSAGE_ERROR,
    Gtk::ButtonsType buttons = Gtk::BUTTONS_OK);

}
}
