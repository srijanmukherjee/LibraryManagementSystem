#pragma once
#include <wx/wxprec.h>

#include "../BookListCtrl.h"
#include "models/Book.h"
#include "controller/Library.h"
#include "spdlog/spdlog.h"

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class BookTabPage : public wxWindow
{
public:
    BookTabPage(wxWindow *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxASCII_STR(wxPanelNameStr));
    ~BookTabPage();

    void Search(wxString code, wxString name, wxString author, wxString publisher);
    void UpdateSearchState(bool all = true, wxString code = wxEmptyString, wxString name = wxEmptyString, wxString author = wxEmptyString, wxString publisher = wxEmptyString);
    void Reload();
    void SetLendBookCallback(std::function<void(std::string)> func);
    void SetDeleteBookCallback(std::function<void(std::string)> func);
    void SetEditBookCallback(std::function<void(std::string)> func);
    
private:
    wxBoxSizer *mainSizer = nullptr;
    wxBoxSizer *messageSizer = nullptr;
    wxStaticText *messageText = nullptr;
    BookListCtrl *listView = nullptr;

    void ShowAllBooks(wxCommandEvent& event);
    void Refresh(wxCommandEvent& event);
    void OnBookListContextMenu(BookListCtrl* list, wxListEvent& event);
    void PopulateList();

    std::function<void(std::string)> LendBookCallback = [](std::string code) {return;};
    std::function<void(std::string)> EditBookCallback = [](std::string code) {return;};
    std::function<void(std::string)> DeleteBookCallback = [](std::string code) {return;};

    struct {
        bool all;
        wxString code;
        wxString name; 
        wxString author; 
        wxString publisher; 
    } mLastSearchState;
};