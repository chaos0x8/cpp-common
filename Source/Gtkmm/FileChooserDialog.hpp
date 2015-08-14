#pragma once

#include <gtkmm.h>

namespace Common
{
namespace Gui
{

class FileChooserDialog : public Gtk::FileChooserDialog
{
public:
    FileChooserDialog(Gtk::Window& parent, const Glib::ustring& prompt, Gtk::FileChooserAction action);

    void addFilter(const std::string& name, const std::string& patern);

    std::string execute();

private:
    void initDefaultButtons();

    std::vector<Glib::RefPtr<Gtk::FileFilter>> filters;
};

}
}
