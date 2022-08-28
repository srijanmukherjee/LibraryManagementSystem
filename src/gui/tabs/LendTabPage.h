#pragma once

#pragma once
#include <wx/wxprec.h>

#include "../BookListCtrl.h"
#include "models/Book.h"
#include "models/Member.h"
#include "controller/Library.h"
#include "spdlog/spdlog.h"

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class LendTabPage : public wxWindow
{
public:
    LendTabPage(wxWindow *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxASCII_STR(wxPanelNameStr));
    ~LendTabPage();
    void AddBook(std::string code);
    void DisplayBookList(bool show = true);
    
private:
    Member *member = nullptr;
    wxBoxSizer *mainSizer = nullptr;
    wxBoxSizer *messageSizer = nullptr;
    wxStaticText *messageText = nullptr;
    BookListCtrl *listView = nullptr;

    wxTextCtrl *memberIdCtrl = nullptr;
    wxButton *memberSearchBtn = nullptr;
    wxBoxSizer *memberInfoSizer = nullptr;
    wxStaticText *memberName = nullptr;
    wxStaticText *memberAddress = nullptr;
    wxStaticText *memberEmail = nullptr;
    wxStaticText *memberPhone = nullptr;
    wxStaticText *memberSince = nullptr;
    wxBoxSizer *bookListSizer = nullptr;
    wxTextCtrl *bookCodeCtrl = nullptr;
    wxButton *bookAddBtn = nullptr;
    wxButton *issueButton = nullptr;

    void OnSearchMember(wxCommandEvent& event);
    void OnBookAdd(wxCommandEvent& event);
    void OnReset(wxCommandEvent& event);
    void OnIssueBook(wxCommandEvent& event);
    void LendButtonUpdateUI(wxUpdateUIEvent& event);
    void Reset();

    void SearchMember(int id);
    void DisplayMemberInfo(bool show = true);

    static void OnBookListContextMenu(BookListCtrl* list, wxListEvent& event);
};