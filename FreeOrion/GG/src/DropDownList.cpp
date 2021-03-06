/* GG is a GUI for SDL and OpenGL.
   Copyright (C) 2003-2008 T. Zachary Laine

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1
   of the License, or (at your option) any later version.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
    
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA

   If you do not wish to comply with the terms of the LGPL please
   contact the author as other terms are available for a fee.
    
   Zach Laine
   whatwasthataddress@gmail.com */

#include <GG/DropDownList.h>

#include <GG/GUI.h>
#include <GG/DrawUtil.h>
#include <GG/Scroll.h>
#include <GG/StyleFactory.h>
#include <GG/WndEvent.h>


using namespace GG;

class ModalListPicker : public Wnd
{
public:
    typedef ListBox::iterator iterator;
    typedef boost::signals2::signal<void (iterator)>   SelChangedSignalType;

    ModalListPicker(X w, Y drop_ht, Clr color, const Wnd* relative_to_wnd);

    virtual void LClick(const Pt& pt, Flags<ModKey> mod_keys);
    virtual void ModalInit();

    ListBox* LB()
    { return m_lb_wnd; }

    const ListBox* LB() const
    { return m_lb_wnd; }

    mutable SelChangedSignalType SelChangedSignal; ///< the selection change signal object for this DropDownList

private:
    void LBSelChangedSlot(const ListBox::SelectionSet& rows);

    void LBLeftClickSlot(ListBox::iterator it, const Pt&);

    ListBox*    m_lb_wnd;
    const Wnd*  m_relative_to_wnd;
};

namespace {
    struct DropDownListSelChangedEcho
    {
        DropDownListSelChangedEcho(const DropDownList& drop_list) :
            m_drop_list(drop_list)
        {}
        void operator()(const DropDownList::iterator& it)
        {
            std::cerr << "GG SIGNAL : DropDownList::SelChangedSignal(row="
                      << m_drop_list.IteratorToIndex(it)
                      << ")\n";
        }
        const DropDownList& m_drop_list;
    };

    struct ModalListPickerSelChangedEcho
    {
        ModalListPickerSelChangedEcho(ModalListPicker& picker) :
            m_picker(picker)
        {}
        void operator()(const ListBox::iterator& it)
        {
            std::cerr << "GG SIGNAL : ModalListPicker::SelChangedSignal(row="
                      << std::distance(m_picker.LB()->begin(), it)
                      << ")\n";
        }
        ModalListPicker& m_picker;
    };

    const int BORDER_THICK = 2; // should be the same as the BORDER_THICK value in GGListBox.h
}

////////////////////////////////////////////////
// ModalListPicker
////////////////////////////////////////////////
ModalListPicker::ModalListPicker(X w, Y drop_ht, Clr color, const Wnd* relative_to_wnd) :
    Wnd(X0, Y0, GUI::GetGUI()->AppWidth(), GUI::GetGUI()->AppHeight(), INTERACTIVE | MODAL),
    m_lb_wnd(GetStyleFactory()->NewDropDownListListBox(X0, Y0, w, drop_ht, color, color)),
    m_relative_to_wnd(relative_to_wnd)
{
    Connect(m_lb_wnd->SelChangedSignal,     &ModalListPicker::LBSelChangedSlot, this);
    Connect(m_lb_wnd->LeftClickedSignal,    &ModalListPicker::LBLeftClickSlot,  this);
    AttachChild(m_lb_wnd);

    if (INSTRUMENT_ALL_SIGNALS)
        Connect(SelChangedSignal, ModalListPickerSelChangedEcho(*this));
}

void ModalListPicker::LClick(const Pt& pt, Flags<ModKey> mod_keys)
{ m_done = true; }

void ModalListPicker::ModalInit()
{
    if (m_relative_to_wnd)
        m_lb_wnd->MoveTo(Pt(m_relative_to_wnd->Left(), m_relative_to_wnd->Bottom()));
    Show();
}

void ModalListPicker::LBSelChangedSlot(const ListBox::SelectionSet& rows)
{
    Hide();
    if (rows.empty()) {
        SelChangedSignal(m_lb_wnd->end());
    } else {
        ListBox::iterator sel_it = *rows.begin();
        SelChangedSignal(sel_it);
    }
    m_done = true;
}

void ModalListPicker::LBLeftClickSlot(ListBox::iterator it, const Pt&)
{
    Hide();
    m_done = true;
}


////////////////////////////////////////////////
// GG::DropDownList
////////////////////////////////////////////////
DropDownList::DropDownList() :
    Control(),
    m_modal_picker(0)
{}

DropDownList::DropDownList(X x, Y y, X w, Y h, Y drop_ht, Clr color, Flags<WndFlag> flags/* = INTERACTIVE*/) :
    Control(x, y, w, h, flags),
    m_modal_picker(new ModalListPicker(w, drop_ht, color, this))
{
    SetStyle(LIST_SINGLESEL);

    // adjust size to keep correct height based on row height, etc.
    Wnd::SizeMove(Pt(x, y),
                  Pt(x + Size().x,
                     y + h + 2 * static_cast<int>(LB()->CellMargin()) + 2 * BORDER_THICK));

    Connect(m_modal_picker->SelChangedSignal, SelChangedSignal);

    if (INSTRUMENT_ALL_SIGNALS)
        Connect(SelChangedSignal, DropDownListSelChangedEcho(*this));
}

DropDownList::~DropDownList() {
    if (m_modal_picker)
        m_modal_picker->EndRun();
    DetachChild(m_modal_picker);
    delete m_modal_picker;
}

DropDownList::iterator DropDownList::CurrentItem() const
{
    if (m_modal_picker->LB()->Selections().empty())
        return m_modal_picker->LB()->end();
    else
        return *m_modal_picker->LB()->Selections().begin();
}

std::size_t DropDownList::CurrentItemIndex() const
{ return IteratorToIndex(CurrentItem()); }

std::size_t DropDownList::IteratorToIndex(iterator it) const
{ return it == m_modal_picker->LB()->end() ? -1 : std::distance(m_modal_picker->LB()->begin(), it); }

DropDownList::iterator DropDownList::IndexToIterator(std::size_t n) const
{ return n < LB()->NumRows() ? boost::next(m_modal_picker->LB()->begin(), n) : m_modal_picker->LB()->end(); }

bool DropDownList::Empty() const
{ return LB()->Empty(); }

DropDownList::const_iterator DropDownList::begin() const
{ return LB()->begin(); }

DropDownList::const_iterator DropDownList::end() const
{ return LB()->end(); }

DropDownList::const_reverse_iterator DropDownList::rbegin() const
{ return LB()->rbegin(); }

DropDownList::const_reverse_iterator DropDownList::rend() const
{ return LB()->rend(); }

const DropDownList::Row& DropDownList::GetRow(std::size_t n) const
{ return LB()->GetRow(n); }

bool DropDownList::Selected(iterator it) const
{ return LB()->Selected(it); }

bool DropDownList::Selected(std::size_t n) const
{ return n < LB()->NumRows() ? LB()->Selected(boost::next(m_modal_picker->LB()->begin(), n)) : false; }

Clr DropDownList::InteriorColor() const
{ return LB()->InteriorColor(); }

Y DropDownList::DropHeight() const
{ return LB()->Height(); }

Flags<ListBoxStyle> DropDownList::Style() const
{ return LB()->Style(); }

std::size_t DropDownList::NumRows() const
{ return LB()->NumRows(); }

std::size_t DropDownList::NumCols() const
{ return LB()->NumCols(); }

std::size_t DropDownList::SortCol() const
{ return LB()->SortCol(); }

X DropDownList::ColWidth(std::size_t n) const
{ return LB()->ColWidth(n); }

Alignment DropDownList::ColAlignment(std::size_t n) const
{ return LB()->ColAlignment(n); }

Alignment DropDownList::RowAlignment(iterator it) const
{ return LB()->RowAlignment(it); }

Pt DropDownList::ClientUpperLeft() const
{ return UpperLeft() + Pt(X(BORDER_THICK), Y(BORDER_THICK)); }

Pt DropDownList::ClientLowerRight() const
{ return LowerRight() - Pt(X(BORDER_THICK), Y(BORDER_THICK)); }

void DropDownList::Render()
{
    // draw beveled rectangle around client area
    Pt ul = UpperLeft(), lr = LowerRight();
    Clr color_to_use = Disabled() ? DisabledColor(LB()->Color()) : LB()->Color();
    Clr int_color_to_use = Disabled() ? DisabledColor(LB()->m_int_color) : LB()->m_int_color;

    BeveledRectangle(ul, lr, int_color_to_use, color_to_use, false, BORDER_THICK);

    // Draw the ListBox::Row of currently displayed item, if any.
    if (CurrentItem() != LB()->end()) {
        Row* current_item = *CurrentItem();
        Pt offset = ClientUpperLeft() - current_item->UpperLeft();
        bool visible = current_item->Visible();
        current_item->OffsetMove(offset);
        if (!visible)
            current_item->Show();
        BeginClipping();
        GUI::GetGUI()->RenderWindow(current_item);
        EndClipping();
        current_item->OffsetMove(-offset);
        if (!visible)
            current_item->Hide();
    }
}

void DropDownList::SizeMove(const Pt& ul, const Pt& lr)
{
    // adjust size to keep correct height based on row height, etc.
    Wnd::SizeMove(ul, lr);
    LB()->SizeMove(Pt(X0, Height()), Pt(Width(), Height() + LB()->Height()));
}

void DropDownList::SetColor(Clr c)
{ LB()->SetColor(c); }

DropDownList::iterator DropDownList::Insert(Row* row, iterator it, bool signal/* = true*/)
{
    row->SetDragDropDataType("");
    return LB()->Insert(row, it, signal);
}

DropDownList::iterator DropDownList::Insert(Row* row, bool signal/* = true*/)
{
    row->SetDragDropDataType("");
    return LB()->Insert(row, signal);
}

void DropDownList::Insert(const std::vector<Row*>& rows, iterator it, bool signal/* = true*/)
{
    for (std::vector<Row*>::const_iterator rows_it = rows.begin();
         rows_it != rows.end(); ++rows_it)
    { (*rows_it)->SetDragDropDataType(""); }
    LB()->Insert(rows, it, signal);
}

void DropDownList::Insert(const std::vector<Row*>& rows, bool signal/* = true*/)
{
    for (std::vector<Row*>::const_iterator rows_it = rows.begin();
         rows_it != rows.end(); ++rows_it)
    { (*rows_it)->SetDragDropDataType(""); }
    LB()->Insert(rows, signal);
}

DropDownList::Row* DropDownList::Erase(iterator it, bool signal/* = false*/)
{ return LB()->Erase(it, signal); }

void DropDownList::Clear()
{
    m_modal_picker->EndRun();
    LB()->Clear();
}

DropDownList::iterator DropDownList::begin()
{ return LB()->begin(); }

DropDownList::iterator DropDownList::end()
{ return LB()->end(); }

DropDownList::reverse_iterator DropDownList::rbegin()
{ return LB()->rbegin(); }

DropDownList::reverse_iterator DropDownList::rend()
{ return LB()->rend(); }

DropDownList::Row& DropDownList::GetRow(std::size_t n)
{ return LB()->GetRow(n); }

void DropDownList::Select(iterator it)
{ SelectImpl(it, false); }

void DropDownList::Select(std::size_t n)
{ SelectImpl(n < LB()->NumRows() ? boost::next(LB()->begin(), n) : LB()->end(), false); }

void DropDownList::SetInteriorColor(Clr c)
{ LB()->SetInteriorColor(c); }

void DropDownList::SetDropHeight(Y h)
{ LB()->Resize(Pt(Width(), h)); }

void DropDownList::SetStyle(Flags<ListBoxStyle> s)
{
    s &= ~(LIST_NOSEL | LIST_QUICKSEL | LIST_USERDELETE | LIST_BROWSEUPDATES);
    s |= LIST_SINGLESEL;
    LB()->SetStyle(s);
}

void DropDownList::SetNumCols(std::size_t n)
{ LB()->SetNumCols(n); }

void DropDownList::SetSortCol(std::size_t n)
{ LB()->SetSortCol(n); }

void DropDownList::SetColWidth(std::size_t n, X w)
{ LB()->SetColWidth(n, w); }

void DropDownList::LockColWidths()
{ LB()->LockColWidths(); }

void DropDownList::UnLockColWidths()
{ LB()->UnLockColWidths(); }

void DropDownList::SetColAlignment(std::size_t n, Alignment align) 
{ LB()->SetColAlignment(n, align); }

void DropDownList::SetRowAlignment(iterator it, Alignment align) 
{ LB()->SetRowAlignment(it, align); }

void DropDownList::LClick(const Pt& pt, Flags<ModKey> mod_keys)
{
    if (Disabled())
        return;

    const ListBox::SelectionSet& LB_sels = LB()->Selections();
    if (!LB_sels.empty()) {
        if (LB()->m_vscroll) {
            LB()->m_vscroll->ScrollTo(0);
            SignalScroll(*LB()->m_vscroll, true);
        }
    }
    LB()->m_first_col_shown = 0;

    m_modal_picker->Run();
}

void DropDownList::KeyPress(Key key, boost::uint32_t key_code_point, Flags<ModKey> mod_keys)
{
    if (!Disabled()) {
        switch (key) {
        case GGK_UP: // arrow-up (not numpad arrow)
            if (CurrentItem() != LB()->end() && CurrentItem() != LB()->begin())
                SelectImpl(boost::prior(CurrentItem()), true);
            break;
        case GGK_DOWN: // arrow-down (not numpad arrow)
            if (CurrentItem() != LB()->end() && CurrentItem() != --LB()->end())
                SelectImpl(boost::next(CurrentItem()), true);
            break;
        case GGK_PAGEUP: // page up key (not numpad key)
            if (LB()->NumRows() && CurrentItem() != LB()->end()) {
                std::size_t i = 10;
                iterator it = CurrentItem();
                while (i && it != LB()->begin()) {
                    --it;
                    --i;
                }
                SelectImpl(it, true);
            }
            break;
        case GGK_PAGEDOWN: // page down key (not numpad key)
            if (LB()->NumRows()) {
                std::size_t i = 10;
                iterator it = CurrentItem();
                while (i && it != --LB()->end()) {
                    ++it;
                    ++i;
                }
                SelectImpl(it, true);
            }
            break;
        case GGK_HOME: // home key (not numpad)
            if (LB()->NumRows())
                SelectImpl(LB()->begin(), true);
            break;
        case GGK_END: // end key (not numpad)
            if (LB()->NumRows() && !LB()->Empty())
                SelectImpl(--LB()->end(), true);
            break;
        default:
            Control::KeyPress(key, key_code_point, mod_keys);
        }
    } else {
        Control::KeyPress(key, key_code_point, mod_keys);
    }
}

ListBox* DropDownList::LB()
{ return m_modal_picker->LB(); }

const ListBox* DropDownList::LB() const
{ return m_modal_picker->LB(); }

void DropDownList::SelectImpl(iterator it, bool signal)
{
    iterator old_m_current_item = CurrentItem();
    if (it == LB()->end()) {
        LB()->DeselectAll();
    } else {
        LB()->SelectRow(it);
    }

    if (signal && CurrentItem() != old_m_current_item)
        SelChangedSignal(CurrentItem());
}
