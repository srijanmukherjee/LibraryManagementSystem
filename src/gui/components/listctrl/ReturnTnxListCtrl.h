#pragma once
#include <wx/wxprec.h>
#include <wx/listctrl.h>
#include <vector>

#include "models/History.h"
#include "controller/Library.h"

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class ReturnTnxListCtrl : public wxListCtrl
{
public:
    ReturnTnxListCtrl(wxWindow *parent,
                 wxWindowID winid = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxLC_REPORT | wxLC_VIRTUAL,
                 const wxValidator& validator = wxDefaultValidator,
                 const wxString &name = wxASCII_STR(wxListCtrlNameStr));
    ~ReturnTnxListCtrl();

    virtual wxString OnGetItemText(long index, long column) const wxOVERRIDE;
    void RefreshAfterUpdate();
    void Insert(History &history, bool unique = false);
    void Remove(long index);
    void Clear();

    std::vector<History> items;

private:
    void OnContextMenu(wxListEvent &event);
    bool mSortAscending = true;
    void SortByColumn(int column);
    long GetFirstSelectedIndex();
    long FindIndexOfId(int id);
};