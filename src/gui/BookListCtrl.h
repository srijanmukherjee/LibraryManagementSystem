#pragma once
#include <wx/wxprec.h>
#include <wx/listctrl.h>
#include <vector>
#include <functional>

#include "models/Book.h"
#include "controller/Library.h"
#include "bookitem.h"

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class BookListCtrl : public wxListCtrl
{
public:
    BookListCtrl(wxWindow *parent,
                 wxWindowID winid = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxLC_REPORT | wxLC_VIRTUAL,
                 const wxValidator& validator = wxDefaultValidator,
                 const wxString &name = wxASCII_STR(wxListCtrlNameStr));
    ~BookListCtrl();

    virtual wxString OnGetItemText(long index, long column) const wxOVERRIDE;
    void RefreshAfterUpdate();
    void Insert(Book &book, bool unique = false);
    void Remove(long index);
    void Clear();

    void SetContextMenuCallBack(std::function<void(BookListCtrl*, wxListEvent&)> func);

    std::vector<BookItem> items;

private:
    void OnContextMenu(wxListEvent &event);
    bool mSortAscending = true;
    void SortByColumn(int column);
    long GetFirstSelectedIndex();
    long FindIndexOfCode(std::string code);

    std::function<void(BookListCtrl*, wxListEvent&)> contextMenuCallback = [] (BookListCtrl*, wxListEvent&) { return; };
};