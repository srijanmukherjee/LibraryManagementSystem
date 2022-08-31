#pragma once
#include <wx/wxprec.h>
#include <functional>

#include "../BookListCtrl.h"
#include "models/Book.h"
#include "controller/Library.h"
#include "../components/listctrl/MemberListCtrl.h"
#include "spdlog/spdlog.h"

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class MemberTabPage : public wxWindow
{
public:
    MemberTabPage(wxWindow *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxASCII_STR(wxPanelNameStr));
    ~MemberTabPage();

    void Search(wxString name, wxString email, wxString phonenumber, wxString address);
    void UpdateSearchState(bool all = true, wxString name = wxEmptyString, wxString email = wxEmptyString, wxString phonenumber = wxEmptyString, wxString address = wxEmptyString);
    void Reload();
    void SetUpdateMemberCallback(std::function<void(int)> func);
    void SetUnregisterMemberCallback(std::function<void(int)> func);
    
private:
    wxBoxSizer *mainSizer = nullptr;
    wxBoxSizer *messageSizer = nullptr;
    wxStaticText *messageText = nullptr;
    MemberListCtrl *listView = nullptr;

    void OnMemberListContextMenu(MemberListCtrl *list, wxListEvent& event);
    void ShowAllMembers(wxCommandEvent& event);
    void Refresh(wxCommandEvent& event);
    void PopulateList();

    struct {
        bool all;
        wxString name;
        wxString email; 
        wxString phonenumber; 
        wxString address;
    } mLastSearchState;

    std::function<void(int)> EditMemberCallback = [](int id) {return;};
    std::function<void(int)> UnregisterMemberCallback = [](int id) {return;};
};