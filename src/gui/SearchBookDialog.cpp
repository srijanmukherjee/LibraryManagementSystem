#include "SearchBookDialog.h"
#include <wx/valtext.h>

SearchBookDialog::SearchBookDialog(wxWindow *parent,
                                   wxWindowID id,
                                   const wxString &title,
                                   const wxPoint &pos,
                                   const wxSize &size,
                                   long style,
                                   const wxString &name)
    : wxDialog(parent, id, title, pos, size, style, name)
{
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *codeFieldSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *codeFieldLabel = new wxStaticText(this, wxID_ANY, _("Code"));
    codeFieldCtrl = new wxTextCtrl(this, wxID_ANY);
    codeFieldCtrl->SetValidator(wxTextValidator(wxFILTER_ALPHANUMERIC));
    codeFieldLabel->SetMinSize(wxSize(70, codeFieldCtrl->GetSize().y));

    codeFieldSizer->Add(codeFieldLabel, 0, wxCENTER);
    codeFieldSizer->Add(codeFieldCtrl, 1);
    mainSizer->Add(codeFieldSizer, 0, wxEXPAND | wxALL, 5);

    wxBoxSizer *nameFieldSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *nameFieldLabel = new wxStaticText(this, wxID_ANY, _("Name"));
    nameFieldCtrl = new wxTextCtrl(this, wxID_ANY);
    nameFieldLabel->SetMinSize(wxSize(70, nameFieldCtrl->GetSize().y));
    
    nameFieldSizer->Add(nameFieldLabel, 0, wxCENTER);
    nameFieldSizer->Add(nameFieldCtrl, 1);
    mainSizer->Add(nameFieldSizer, 0, wxEXPAND | wxALL, 5);

    wxBoxSizer *authorFieldSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *authorFieldLabel = new wxStaticText(this, wxID_ANY, _("Author"));
    authorFieldCtrl = new wxTextCtrl(this, wxID_ANY);
    authorFieldLabel->SetMinSize(wxSize(70, authorFieldCtrl->GetSize().y));
    
    authorFieldSizer->Add(authorFieldLabel, 0, wxCENTER);
    authorFieldSizer->Add(authorFieldCtrl, 1);
    mainSizer->Add(authorFieldSizer, 0, wxEXPAND | wxALL, 5);

    wxBoxSizer *publisherFieldSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *publisherFieldLabel = new wxStaticText(this, wxID_ANY, _("Publisher"));
    publisherFieldCtrl = new wxTextCtrl(this, wxID_ANY);
    publisherFieldLabel->SetMinSize(wxSize(70, publisherFieldCtrl->GetSize().y));
    
    publisherFieldSizer->Add(publisherFieldLabel, 0, wxCENTER);
    publisherFieldSizer->Add(publisherFieldCtrl, 1);
    mainSizer->Add(publisherFieldSizer, 0, wxEXPAND | wxALL, 5);
    
    wxSizer *buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxBOTTOM, 5);

    SetSizer(mainSizer);
    SetMinSize(wxSize(400, 100));
    Fit();
}

SearchBookDialog::~SearchBookDialog()
{
}