#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class AddMemberDialog : public wxDialog
{
public:
    AddMemberDialog(wxWindow *parent,
                  wxWindowID id,
                  const wxString &title,
                  const wxPoint &pos = wxDefaultPosition,
                  const wxSize &size = wxDefaultSize,
                  long style = wxDEFAULT_DIALOG_STYLE,
                  const wxString &name = wxASCII_STR(wxDialogNameStr));
    
    wxString GetFirstName() const { return firstNameFieldCtrl->GetValue(); }
    wxString GetLastName() const { return lastNameFieldCtrl->GetValue(); }
    wxString GetEmail() const { return emailFieldCtrl->GetValue(); }
    wxString GetPhonenumber() const { return phonenumberFieldCtrl->GetValue(); }
    wxString GetAddress() const { return addressFieldCtrl->GetValue(); }
    virtual ~AddMemberDialog();

private:
    wxTextCtrl *firstNameFieldCtrl;
    wxTextCtrl *lastNameFieldCtrl;
    wxTextCtrl *emailFieldCtrl;
    wxTextCtrl *phonenumberFieldCtrl;
    wxTextCtrl *addressFieldCtrl;
};