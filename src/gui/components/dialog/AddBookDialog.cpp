#include "AddBookDialog.h"

#include <wx/valnum.h>
#include <wx/valgen.h>
#include <wx/valtext.h>

AddBookDialog::AddBookDialog(
    wxWindow *parent,
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
    nameFieldCtrl->SetValidator(wxTextValidator(wxFILTER_EMPTY));
    nameFieldLabel->SetMinSize(wxSize(70, nameFieldCtrl->GetSize().y));
    nameFieldSizer->Add(nameFieldLabel, 0, wxCENTER);
    nameFieldSizer->Add(nameFieldCtrl, 1);
    mainSizer->Add(nameFieldSizer, 0, wxEXPAND | wxALL, 5);

    wxBoxSizer *authorFieldSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *authorFieldLabel = new wxStaticText(this, wxID_ANY, _("Author"));
    authorFieldCtrl = new wxTextCtrl(this, wxID_ANY);
    authorFieldCtrl->SetValidator(wxTextValidator(wxFILTER_EMPTY));
    authorFieldLabel->SetMinSize(wxSize(70, authorFieldCtrl->GetSize().y));
    authorFieldSizer->Add(authorFieldLabel, 0, wxCENTER);
    authorFieldSizer->Add(authorFieldCtrl, 1);
    mainSizer->Add(authorFieldSizer, 0, wxEXPAND | wxALL, 5);

    wxBoxSizer *publisherFieldSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *publisherFieldLabel = new wxStaticText(this, wxID_ANY, _("Publisher"));
    publisherFieldCtrl = new wxTextCtrl(this, wxID_ANY);
    publisherFieldCtrl->SetValidator(wxTextValidator(wxFILTER_EMPTY));
    publisherFieldLabel->SetMinSize(wxSize(70, publisherFieldCtrl->GetSize().y));
    publisherFieldSizer->Add(publisherFieldLabel, 0, wxCENTER);
    publisherFieldSizer->Add(publisherFieldCtrl, 1);
    mainSizer->Add(publisherFieldSizer, 0, wxEXPAND | wxALL, 5);

    wxBoxSizer *quantityFieldSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *quantityFieldLabel = new wxStaticText(this, wxID_ANY, _("Quantity"));
    quantityFieldCtrl = new wxTextCtrl(this, wxID_ANY);
    wxIntegerValidator<int> quantityValidator;
    quantityValidator.SetMin(1);
    quantityFieldCtrl->SetValidator(quantityValidator);
    quantityFieldLabel->SetMinSize(wxSize(70, quantityFieldCtrl->GetSize().y));
    quantityFieldSizer->Add(quantityFieldLabel, 0, wxCENTER);
    quantityFieldSizer->Add(quantityFieldCtrl, 1);
    mainSizer->Add(quantityFieldSizer, 0, wxEXPAND | wxALL, 5);

    wxSizer *buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxALL, 5);

    SetSizer(mainSizer);
    SetMinSize(wxSize(400, 100));
    Fit();
}

AddBookDialog::~AddBookDialog()
{

}