#include <Gtkmm/FileChooserDialog.hpp>
#include <Common/Exceptions/OperationAborted.hpp>

namespace Common
{
namespace Gui
{
namespace Detail
{

Glib::RefPtr<Gtk::FileFilter> createFilter(const std::string& decription, const std::string& patern)
{
    Glib::RefPtr<Gtk::FileFilter> filter = Gtk::FileFilter::create();
    filter->set_name(decription);
    filter->add_pattern(patern);
    return filter;
}

class FilterActivator
{
public:
    FilterActivator(Gtk::FileChooserDialog& target)
        : target(target)
    {
    }

    ~FilterActivator()
    {
        for (Glib::RefPtr<Gtk::FileFilter> f : activeFilters)
            target.remove_filter(f);
    }

    void operator()(const std::vector<Glib::RefPtr<Gtk::FileFilter>>& filters)
    {
        activeFilters = filters;
        if (activeFilters.empty())
            activeFilters.push_back(createFilter("Any files", "*"));
        for (Glib::RefPtr<Gtk::FileFilter> f : activeFilters)
            target.add_filter(f);
    }

private:
    Gtk::FileChooserDialog& target;

    std::vector<Glib::RefPtr<Gtk::FileFilter>> activeFilters;
};

}

FileChooserDialog::FileChooserDialog(Gtk::Window& parent, const Glib::ustring& prompt, Gtk::FileChooserAction action)
    : Gtk::FileChooserDialog(prompt, action)
{
    set_transient_for(parent);

    initDefaultButtons();
}

void FileChooserDialog::addFilter(const std::string& name, const std::string& patern)
{
    filters.push_back(Detail::createFilter(name, patern));
}

std::string FileChooserDialog::execute()
{
    Detail::FilterActivator activator(*this);
    activator(filters);

    if (run() != Gtk::RESPONSE_OK)
    {
        throw Common::Exceptions::OperationAborted("Picking file.");
    }
    return get_filename();
}

void FileChooserDialog::initDefaultButtons()
{
    add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
}

}
}
