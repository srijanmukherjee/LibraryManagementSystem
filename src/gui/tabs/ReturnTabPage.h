#pragma once
#include <wx/wxprec.h>

#include "../BookListCtrl.h"
#include "../components/listctrl/ReturnTnxListCtrl.h"
#include "models/Book.h"
#include "models/Member.h"
#include "models/History.h"
#include "controller/Library.h"
#include "spdlog/spdlog.h"

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class ReturnTabPage : public wxWindow
{
public:
    ReturnTabPage(wxWindow *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxASCII_STR(wxPanelNameStr));
    ~ReturnTabPage();
    void DisplayBookList(bool show = true);
    
private:
    Member *member = nullptr;
    wxBoxSizer *mainSizer = nullptr;
    wxBoxSizer *messageSizer = nullptr;
    wxStaticText *messageText = nullptr;
    ReturnTnxListCtrl *listView = nullptr;

    wxTextCtrl *memberIdCtrl = nullptr;
    wxButton *memberSearchBtn = nullptr;
    wxBoxSizer *memberInfoSizer = nullptr;
    wxStaticText *memberName = nullptr;
    wxStaticText *memberAddress = nullptr;
    wxStaticText *memberEmail = nullptr;
    wxStaticText *memberPhone = nullptr;
    wxStaticText *memberSince = nullptr;
    wxBoxSizer *bookListSizer = nullptr;
    wxButton *returnButton = nullptr;

    void OnSearchMember(wxCommandEvent& event);
    void OnReset(wxCommandEvent& event);
    void OnReturnBook(wxCommandEvent& event);
    void ReturnButtonUpdateUI(wxUpdateUIEvent& event);
    void Reset();
    void LoadIssuedBooks();

    void SearchMember(int id);
    void DisplayMemberInfo(bool show = true);
};