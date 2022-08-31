#pragma once
#include <wx/wxprec.h>
#include <wx/listctrl.h>
#include <wx/notebook.h>

#include "sqlite3.h"
#include "models/Book.h"
#include "tabs/BookTabPage.h"
#include "tabs/LendTabPage.h"
#include "tabs/MemberTabPage.h"
#include "tabs/ReturnTabPage.h"

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class MainWindow : public wxFrame
{
public:
    MainWindow(wxWindow *parent,
               wxWindowID id,
               const wxString& title,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxDEFAULT_FRAME_STYLE,
               const wxString& name = wxASCII_STR(wxFrameNameStr));

    void OnClose(wxCloseEvent& event);
    void OnIdle(wxIdleEvent& event);
    void OnUpdateOKButton(wxUpdateUIEvent& event);
    void LendBook(std::string &code);
    void DeleteBook(std::string &code);
    void EditBook(std::string &code);
        
    wxNotebook *tabNotebook = nullptr;

private:
    wxButton *searchBookButton;
    wxButton *searchMemberButton;
    wxButton *lendBookButton;
    wxButton *returnBookButton;
    wxButton *addBookButton;
    wxButton *addMemberButton;

    BookTabPage *booksPage  = nullptr;
    MemberTabPage *memberPage = nullptr;
    LendTabPage *lendPage   = nullptr;
    ReturnTabPage *returnPage   = nullptr;
    sqlite3* db = NULL;

    void OnSearchBook(wxCommandEvent& event);
    void OnSearchMember(wxCommandEvent& event);
    void OnAddBook(wxCommandEvent& event);
    void OnReturnBook(wxCommandEvent& event);
    void OnAddMember(wxCommandEvent& event);
    void OnLendButtonPress(wxCommandEvent& event);
    void CreateBook(std::string name, std::string author, std::string publisher, int count);
    void UpdateBook(std::string code, std::string name, std::string author, std::string publisher, int count);
    void CreateMember(std::string fname, std::string lname, std::string email, std::string phonenumber, std::string address);
    bool UpdateMember(int id, std::string fname, std::string lname, std::string email, std::string phonenumber, std::string address);
    void OnUpdateMember(int id);
    void OnUnregisterMember(int id);
    void OnRegisterMember(int id);

    wxButton* CreateControlButton(wxWindow *parent, wxWindowID id, const wxString &label);

    DECLARE_EVENT_TABLE()
};