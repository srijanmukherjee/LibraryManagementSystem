#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class SearchMemberDialog : public wxDialog
{
public:
    SearchMemberDialog(wxWindow *parent,
                  wxWindowID id,
                  const wxString &title,
                  const wxPoint &pos = wxDefaultPosition,
                  const wxSize &size = wxDefaultSize,
                  long style = wxDEFAULT_DIALOG_STYLE,
                  const wxString &name = wxASCII_STR(wxDialogNameStr));
    
    wxString GetName() const { return nameFieldCtrl->GetValue(); }
    wxString GetEmail() const { return emailFieldCtrl->GetValue(); }
    wxString GetPhonenumber() const { return phonenumberFieldCtrl->GetValue(); }
    wxString GetAddress() const { return addressFieldCtrl->GetValue(); }
    virtual ~SearchMemberDialog();

private:
    wxTextCtrl *nameFieldCtrl;
    wxTextCtrl *emailFieldCtrl;
    wxTextCtrl *phonenumberFieldCtrl;
    wxTextCtrl *addressFieldCtrl;
};