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

#include <Gtkmm/FileChooserDialog.hpp>
#include <Common/Exceptions/OperationAborted.hpp>

namespace Common
{
namespace Gui
{
namespace Detail
{

Glib::RefPtr<Gtk::FileFilter> createFilter(const std::string& decription, const std::string& pattern)
{
    Glib::RefPtr<Gtk::FileFilter> filter = Gtk::FileFilter::create();
    filter->set_name(decription);
    filter->add_pattern(pattern);
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

void FileChooserDialog::addFilter(const std::string& name, const std::string& pattern)
{
    filters.push_back(Detail::createFilter(name, pattern));
}

void FileChooserDialog::setFileName(const std::string& fileName)
{
  set_filename(fileName);
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
