#include "SearchMemberDialog.h"
#include <wx/valtext.h>

SearchMemberDialog::SearchMemberDialog(wxWindow *parent,
                                   wxWindowID id,
                                   const wxString &title,
                                   const wxPoint &pos,
                                   const wxSize &size,
                                   long style,
                                   const wxString &name)
    : wxDialog(parent, id, title, pos, size, style, name)
{
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *nameFieldSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *nameFieldLabel = new wxStaticText(this, wxID_ANY, _("Name"));
    nameFieldCtrl = new wxTextCtrl(this, wxID_ANY);
    nameFieldLabel->SetMinSize(wxSize(70, nameFieldCtrl->GetSize().y));
    
    nameFieldSizer->Add(nameFieldLabel, 0, wxCENTER);
    nameFieldSizer->Add(nameFieldCtrl, 1);
    mainSizer->Add(nameFieldSizer, 0, wxEXPAND | wxALL, 5);

    wxBoxSizer *emailFieldSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *emailFieldLabel = new wxStaticText(this, wxID_ANY, _("Email"));
    emailFieldCtrl = new wxTextCtrl(this, wxID_ANY);
    emailFieldLabel->SetMinSize(wxSize(70, emailFieldCtrl->GetSize().y));
    
    emailFieldSizer->Add(emailFieldLabel, 0, wxCENTER);
    emailFieldSizer->Add(emailFieldCtrl, 1);
    mainSizer->Add(emailFieldSizer, 0, wxEXPAND | wxALL, 5);

    wxBoxSizer *phonenumberFieldSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *phonenumberFieldLabel = new wxStaticText(this, wxID_ANY, _("Phone"));
    phonenumberFieldCtrl = new wxTextCtrl(this, wxID_ANY);
    phonenumberFieldLabel->SetMinSize(wxSize(70, phonenumberFieldCtrl->GetSize().y));
    
    phonenumberFieldSizer->Add(phonenumberFieldLabel, 0, wxCENTER);
    phonenumberFieldSizer->Add(phonenumberFieldCtrl, 1);
    mainSizer->Add(phonenumberFieldSizer, 0, wxEXPAND | wxALL, 5);

    wxBoxSizer *addressFieldSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *addressFieldLabel = new wxStaticText(this, wxID_ANY, _("Address"));
    addressFieldCtrl = new wxTextCtrl(this, wxID_ANY);
    addressFieldLabel->SetMinSize(wxSize(70, addressFieldCtrl->GetSize().y));
    
    addressFieldSizer->Add(addressFieldLabel, 0, wxCENTER);
    addressFieldSizer->Add(addressFieldCtrl, 1);
    mainSizer->Add(addressFieldSizer, 0, wxEXPAND | wxALL, 5);
    
    wxSizer *buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxBOTTOM, 5);

    SetSizer(mainSizer);
    SetMinSize(wxSize(400, 100));
    Fit();
}

SearchMemberDialog::~SearchMemberDialog()
{
}