#pragma once
#include <wx/wxprec.h>
#include <wx/listctrl.h>
#include <vector>
#include <functional>

#include "models/Member.h"
#include "controller/Library.h"

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class MemberListCtrl : public wxListCtrl
{
public:
    MemberListCtrl(wxWindow *parent,
                 wxWindowID winid = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxLC_REPORT | wxLC_VIRTUAL,
                 const wxValidator& validator = wxDefaultValidator,
                 const wxString &name = wxASCII_STR(wxListCtrlNameStr));
    ~MemberListCtrl();

    virtual wxString OnGetItemText(long index, long column) const wxOVERRIDE;
    void RefreshAfterUpdate();
    void Insert(Member &member, bool unique = false);
    void Remove(long index);
    void Clear();
    void SetContextMenuCallBack(std::function<void(MemberListCtrl*, wxListEvent&)> func);

    std::vector<Member> items;

private:
    void OnContextMenu(wxListEvent &event);
    bool mSortAscending = true;
    void SortByColumn(int column);
    long GetFirstSelectedIndex();
    long FindIndexOfId(int id);

    std::function<void(MemberListCtrl*, wxListEvent&)> contextMenuCallback = [] (MemberListCtrl*, wxListEvent&) { return; };
};