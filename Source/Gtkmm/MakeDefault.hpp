#pragma once
#include <gtkmm.h>

namespace Common
{
namespace Gui
{

inline void makeDefault(Gtk::Widget& widget)
{
    widget.set_can_default();
    widget.grab_default();
}

}
}
