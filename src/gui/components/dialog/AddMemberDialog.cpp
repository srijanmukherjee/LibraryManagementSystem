#include "AddMemberDialog.h"

#include <wx/valnum.h>
#include <wx/valgen.h>
#include <wx/valtext.h>

#include "spdlog/spdlog.h"

AddMemberDialog::AddMemberDialog(
    wxWindow *parent,
    wxWindowID id,
    const wxString &title,
    int memberId,
    const wxPoint &pos,
    const wxSize &size,
    long style,
    const wxString &name)
    : wxDialog(parent, id, title, pos, size, style, name)
{
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxString firstName = wxEmptyString;
    wxString lastName = wxEmptyString;
    wxString email = wxEmptyString;
    wxString phone = wxEmptyString;
    wxString address = wxEmptyString;

    if (memberId != -1)
    {
        try {
            Member member = Member::FindById(memberId);
            firstName = member.firstName;
            lastName = member.lastName;
            email = member.email;
            phone = member.phonenumber;
            address = member.address;

        } catch(std::exception &e) {
            spdlog::error(e.what());
        }
    }

    wxBoxSizer *firstNameFieldSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *firstNameFieldLabel = new wxStaticText(this, wxID_ANY, _("First name"));
    firstNameFieldCtrl = new wxTextCtrl(this, wxID_ANY);
    firstNameFieldCtrl->SetValidator(wxTextValidator(wxFILTER_EMPTY));
    firstNameFieldCtrl->SetValue(firstName);
    firstNameFieldLabel->SetMinSize(wxSize(140, firstNameFieldCtrl->GetSize().y));
    firstNameFieldSizer->Add(firstNameFieldLabel, 0, wxCENTER);
    firstNameFieldSizer->Add(firstNameFieldCtrl, 1);
    mainSizer->Add(firstNameFieldSizer, 0, wxEXPAND | wxALL, 5);

    wxBoxSizer *lastNameFieldSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *lastNameFieldLabel = new wxStaticText(this, wxID_ANY, _("Last name"));
    lastNameFieldCtrl = new wxTextCtrl(this, wxID_ANY);
    lastNameFieldCtrl->SetValidator(wxTextValidator(wxFILTER_EMPTY));
    lastNameFieldCtrl->SetValue(lastName);
    lastNameFieldLabel->SetMinSize(wxSize(140, lastNameFieldCtrl->GetSize().y));
    lastNameFieldSizer->Add(lastNameFieldLabel, 0, wxCENTER);
    lastNameFieldSizer->Add(lastNameFieldCtrl, 1);
    mainSizer->Add(lastNameFieldSizer, 0, wxEXPAND | wxALL, 5);

    wxBoxSizer *emailFieldSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *emailFieldLabel = new wxStaticText(this, wxID_ANY, _("Email"));
    emailFieldCtrl = new wxTextCtrl(this, wxID_ANY);
    emailFieldCtrl->SetValue(email);
    emailFieldCtrl->SetValidator(wxTextValidator(wxFILTER_EMPTY));
    emailFieldLabel->SetMinSize(wxSize(140, emailFieldCtrl->GetSize().y));
    emailFieldSizer->Add(emailFieldLabel, 0, wxCENTER);
    emailFieldSizer->Add(emailFieldCtrl, 1);
    mainSizer->Add(emailFieldSizer, 0, wxEXPAND | wxALL, 5);

    wxBoxSizer *phonenumberFieldSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *phonenumberFieldLabel = new wxStaticText(this, wxID_ANY, _("Phone"));
    phonenumberFieldCtrl = new wxTextCtrl(this, wxID_ANY);
    phonenumberFieldCtrl->SetValue(phone);
    phonenumberFieldCtrl->SetValidator(wxTextValidator(wxFILTER_EMPTY));
    phonenumberFieldLabel->SetMinSize(wxSize(140, phonenumberFieldCtrl->GetSize().y));
    phonenumberFieldSizer->Add(phonenumberFieldLabel, 0, wxCENTER);
    phonenumberFieldSizer->Add(phonenumberFieldCtrl, 1);
    mainSizer->Add(phonenumberFieldSizer, 0, wxEXPAND | wxALL, 5);

    wxBoxSizer *addressFieldSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *addressFieldLabel = new wxStaticText(this, wxID_ANY, _("Address"));
    addressFieldCtrl = new wxTextCtrl(this, wxID_ANY);
    addressFieldCtrl->SetValue(address);
    addressFieldCtrl->SetValidator(wxTextValidator(wxFILTER_EMPTY));
    addressFieldLabel->SetMinSize(wxSize(140, addressFieldCtrl->GetSize().y));
    addressFieldSizer->Add(addressFieldLabel, 0, wxCENTER);
    addressFieldSizer->Add(addressFieldCtrl, 1);
    mainSizer->Add(addressFieldSizer, 0, wxEXPAND | wxALL, 5);

    wxSizer *buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxALL, 5);

    SetSizer(mainSizer);
    SetMinSize(wxSize(400, 100));
    Fit();
}

AddMemberDialog::~AddMemberDialog()
{
}