#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class SearchBookDialog : public wxDialog
{
public:
    SearchBookDialog(wxWindow *parent,
                  wxWindowID id,
                  const wxString &title,
                  const wxPoint &pos = wxDefaultPosition,
                  const wxSize &size = wxDefaultSize,
                  long style = wxDEFAULT_DIALOG_STYLE,
                  const wxString &name = wxASCII_STR(wxDialogNameStr));
    
    wxString GetCode() const { return codeFieldCtrl->GetValue(); }
    wxString GetName() const { return nameFieldCtrl->GetValue(); }
    wxString GetAuthor() const { return authorFieldCtrl->GetValue(); }
    wxString GetPublisher() const { return publisherFieldCtrl->GetValue(); }
    virtual ~SearchBookDialog();

private:
    wxTextCtrl *codeFieldCtrl;
    wxTextCtrl *nameFieldCtrl;
    wxTextCtrl *authorFieldCtrl;
    wxTextCtrl *publisherFieldCtrl;
};