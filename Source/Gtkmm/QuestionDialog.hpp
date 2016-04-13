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
#include <Gtkmm/MakeDefault.hpp>
#include <gtkmm.h>
#include <string>
#include <cstdint>
#include <limits>
#include <memory>

namespace Common
{
namespace Gui
{
namespace Detail
{

struct QuestionDialogTraitsBase
{
    static std::string getAnswer(const Gtk::Entry& widget)
    {
        return widget.get_text();
    }

    static double getAnswer(const Gtk::SpinButton& widget)
    {
        return widget.get_value();
    }

    static void prepareWidget(Gtk::Entry& widget, const std::string& defaultValue)
    {
        widget.set_text(defaultValue);
    }

    static void prepareWidget(Gtk::SpinButton& widget, const double& defaultValue)
    {
        widget.set_range(std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max());
        widget.set_value(defaultValue);
        widget.set_increments(1.0, 5.0);
    }
};

template <typename T>
struct QuestionDialogTraits;

template <>
struct QuestionDialogTraits<std::string> : public QuestionDialogTraitsBase
{
    typedef Gtk::Entry widget_type;
};

template <>
struct QuestionDialogTraits<int64_t> : public QuestionDialogTraitsBase
{
    typedef Gtk::SpinButton widget_type;
};

template <>
struct QuestionDialogTraits<size_t> : public QuestionDialogTraitsBase
{
    typedef Gtk::SpinButton widget_type;
};

template <>
struct QuestionDialogTraits<double> : public QuestionDialogTraitsBase
{
    typedef Gtk::SpinButton widget_type;

    static void prepareWidget(widget_type& widget, const double& defaultValue)
    {
        QuestionDialogTraitsBase::prepareWidget(widget, defaultValue);
        widget.set_digits(1);
    }
};

}

template <typename AnswerType>
class QuestionDialog : public Gtk::Dialog
{
public:
    QuestionDialog(Gtk::Window& parent, const std::string& question, const AnswerType& defaultValue)
        : Gtk::Dialog("Question dialog", parent),
          question(question)
    {
        Detail::QuestionDialogTraits<AnswerType>::prepareWidget(widget, defaultValue);

        get_vbox()->pack_start(this->question, Gtk::PACK_SHRINK);
        get_vbox()->pack_start(widget, Gtk::PACK_SHRINK);
        get_vbox()->set_border_width(8);

        makeDefault(*Gtk::manage(add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK)));
        Gtk::manage(add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL));

        widget.set_activates_default();

        show_all_children();
    }

    AnswerType getAnswer() const
    {
        return Detail::QuestionDialogTraits<AnswerType>::getAnswer(widget);
    }

    static AnswerType exec(Gtk::Window& parent, const std::string& question, const AnswerType& defaultValue)
    {
        std::unique_ptr<QuestionDialog<AnswerType>> dialog(new QuestionDialog<AnswerType>(parent, question, defaultValue));
        if (dialog->run() != Gtk::RESPONSE_OK)
            return AnswerType();
        return dialog->getAnswer();
    }

private:
    Gtk::Label question;
    typename Detail::QuestionDialogTraits<AnswerType>::widget_type widget;
};

template <>
class QuestionDialog<Gtk::ResponseType> : public Gtk::Dialog
{
public:
    static Gtk::ResponseType exec(Gtk::Window& parent, const std::string& question, Gtk::ButtonsType buttons = Gtk::BUTTONS_YES_NO);
};

}
}
