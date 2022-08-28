#include "ReturnTabPage.h"

#include <wx/valnum.h>
#include <wx/valtext.h>

#include "../../gui.h"
#include "../id.h"

#include <vector>
#include <thread>

using std::vector;

ReturnTabPage::ReturnTabPage(
        wxWindow *parent,
        wxWindowID id,
        const wxPoint& pos,
        const wxSize& size,
        long style,
        const wxString& name
    ) : wxWindow(parent, id, pos, size, style, name) 
{
    mainSizer = new wxBoxSizer(wxVERTICAL);
    messageSizer = new wxBoxSizer(wxHORIZONTAL);

    // member input
    wxBoxSizer *memberInputSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *memberIdFieldSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *memberIdText = new wxStaticText(this, wxID_ANY, _("Member ID"));
    memberIdCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    wxIntegerValidator<int> memberIdValidator;
    memberIdValidator.SetMin(1);
    memberIdCtrl->SetValidator(memberIdValidator);
    memberIdCtrl->Bind(wxEVT_TEXT_ENTER, &ReturnTabPage::OnSearchMember, this);
    memberSearchBtn = new wxButton(this, wxID_ANY, _("Search"));
    memberSearchBtn->Bind(wxEVT_BUTTON, &ReturnTabPage::OnSearchMember, this);
    memberIdFieldSizer->Add(memberIdText, 0, wxCENTER | wxALL, 5);
    memberIdFieldSizer->Add(memberIdCtrl, 1, wxEXPAND | wxCENTER | wxALL, 5);
    memberIdFieldSizer->Add(memberSearchBtn, 0, wxCENTER | wxALL, 5);
    memberInputSizer->Add(memberIdFieldSizer, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(memberInputSizer, 0, wxEXPAND);

    // member info
    memberInfoSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText *memberInfoTitle = new wxStaticText(this, wxID_ANY, _("Member Details"));
    memberInfoTitle->SetFont(memberInfoTitle->GetFont().Scale(1.2));
    memberInfoSizer->Add(memberInfoTitle, 0, wxALL | wxEXPAND, 10);

    // member name
    wxBoxSizer *memberNameSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *memberNameLabel = new wxStaticText(this, wxID_ANY, _("Name"));
    memberNameLabel->SetMinSize(wxSize(100, memberNameLabel->GetSize().y));
    memberName = new wxStaticText(this, wxID_ANY, _("NA"));
    memberNameSizer->Add(memberNameLabel, 0, wxALL, 5);
    memberNameSizer->Add(memberName, 1, wxALL, 5);
    memberInfoSizer->Add(memberNameSizer, 0, wxEXPAND | wxLEFT, 5);

    // member address
    wxBoxSizer *memberAddressSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *memberAddressLabel = new wxStaticText(this, wxID_ANY, _("Address"));
    memberAddressLabel->SetMinSize(wxSize(100, memberAddressLabel->GetSize().y));
    memberAddress = new wxStaticText(this, wxID_ANY, _("NA"));
    memberAddressSizer->Add(memberAddressLabel, 0, wxALL, 5);
    memberAddressSizer->Add(memberAddress, 1, wxALL, 5);
    memberInfoSizer->Add(memberAddressSizer, 0, wxEXPAND | wxLEFT, 5);

    // member email
    wxBoxSizer *memberEmailSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *memberEmailLabel = new wxStaticText(this, wxID_ANY, _("Email"));
    memberEmailLabel->SetMinSize(wxSize(100, memberEmailLabel->GetSize().y));
    memberEmail = new wxStaticText(this, wxID_ANY, _("NA"));
    memberEmailSizer->Add(memberEmailLabel, 0, wxALL, 5);
    memberEmailSizer->Add(memberEmail, 1, wxALL, 5);
    memberInfoSizer->Add(memberEmailSizer, 0, wxEXPAND | wxLEFT, 5);

    // member phonenumber
    wxBoxSizer *memberPhoneSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *memberPhoneLabel = new wxStaticText(this, wxID_ANY, _("Phone"));
    memberPhoneLabel->SetMinSize(wxSize(100, memberEmailLabel->GetSize().y));
    memberPhone = new wxStaticText(this, wxID_ANY, _("NA"));
    memberPhoneSizer->Add(memberPhoneLabel, 0, wxALL, 5);
    memberPhoneSizer->Add(memberPhone, 1, wxALL, 5);
    memberInfoSizer->Add(memberPhoneSizer, 0, wxEXPAND | wxLEFT, 5);

    // member since
    wxBoxSizer *memberSinceSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *memberSinceLabel = new wxStaticText(this, wxID_ANY, _("Since"));
    memberSinceLabel->SetMinSize(wxSize(100, memberSinceLabel->GetSize().y));
    memberSince = new wxStaticText(this, wxID_ANY, _("NA"));
    memberSinceSizer->Add(memberSinceLabel, 0, wxALL, 5);
    memberSinceSizer->Add(memberSince, 1, wxALL, 5);
    memberInfoSizer->Add(memberSinceSizer, 0, wxEXPAND | wxLEFT, 5);

    mainSizer->Add(memberInfoSizer, 0, wxEXPAND);
    memberInfoSizer->Show(false);

    // book list
    bookListSizer = new wxBoxSizer(wxVERTICAL);
    // title
    wxStaticText *bookListTitle = new wxStaticText(this, wxID_ANY, _("Books Issued"));
    bookListTitle->SetFont(bookListTitle->GetFont().Scale(1.2));
    bookListSizer->Add(bookListTitle, 0, wxALL | wxEXPAND, 10);

    listView = new ReturnTnxListCtrl(this, wxID_ANY);
    bookListSizer->Add(listView, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(bookListSizer, 1, wxEXPAND);
    bookListSizer->Show(false);

    // controls
    wxBoxSizer *controlSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *resetButton = new wxButton(this, wxID_ANY, _("Reset"));
    resetButton->Bind(wxEVT_BUTTON, &ReturnTabPage::OnReset, this);

    this->returnButton = new wxButton(this, wxID_ANY, _("Return books"));
    this->returnButton->Bind(wxEVT_BUTTON, &ReturnTabPage::OnReturnBook, this);
    this->returnButton->Bind(wxEVT_UPDATE_UI, &ReturnTabPage::ReturnButtonUpdateUI, this);
    this->returnButton->Show(false);
    controlSizer->Add(resetButton, 0, wxALL, 5);
    controlSizer->Add(this->returnButton, 0, wxALL, 5);
    mainSizer->Add(controlSizer, 0, wxEXPAND);
    SetSizer(mainSizer);
    Fit();
}

void ReturnTabPage::OnSearchMember(wxCommandEvent& event)
{
    wxString value = memberIdCtrl->GetValue();

    if (value == wxEmptyString)
    {
        wxMessageBox("Member id is empty");
        return;
    }

    int memberId;
    value.ToInt(&memberId);
    this->SearchMember(memberId);
}

void ReturnTabPage::SearchMember(int id)
{
    try
    {
        Member _member = Member::FindById(id);
        if (this->member != nullptr)
        {
            delete this->member;
        }
        this->member = new Member(_member);
        this->LoadIssuedBooks();
        this->DisplayMemberInfo();
        this->DisplayBookList();
        this->returnButton->Show(true);
    }
    catch(const std::exception& e)
    {
        wxMessageBox("Member not found");
    }
}

void ReturnTabPage::DisplayMemberInfo(bool show)
{
    if (!show)
    {
        this->memberInfoSizer->Show(false);
        this->Fit();
        return;
    }

    this->memberInfoSizer->Show(true);
    this->memberName->SetLabelText(wxString::Format("%s %s", this->member->firstName, this->member->lastName));
    this->memberAddress->SetLabelText(this->member->address);
    this->memberEmail->SetLabelText(this->member->email);
    this->memberPhone->SetLabelText(this->member->phonenumber);
    this->memberSince->SetLabelText(this->member->createdOn);
    this->Fit();
}

void ReturnTabPage::DisplayBookList(bool show)
{
    if (!show)
    {
        this->bookListSizer->Show(false);
        this->Fit();
        return;
    }
    this->bookListSizer->Show(true);
    this->Fit();
}

void ReturnTabPage::LoadIssuedBooks()
{
    this->listView->Clear();

    if (this->member == nullptr) return;
    
    Member memberObj(*member);
    vector<History> history = History::GetHistoryOfMember(memberObj, true);
    for (History tnx : history)
    {
        this->listView->Insert(tnx);
    }
}

void ReturnTabPage::Reset()
{
    if (member != nullptr)
    {
        delete member;
        member = nullptr;
    }

    this->memberInfoSizer->Show(false);
    this->bookListSizer->Show(false);
    this->returnButton->Show(false);
    this->memberIdCtrl->SetValue(wxEmptyString);
    this->listView->Clear();
    this->Fit();
}

void ReturnTabPage::OnReset(wxCommandEvent& event)
{
    this->Reset();
}

void ReturnTabPage::OnReturnBook(wxCommandEvent& event)
{
    this->listView->Disable();

    long item = -1;

    while ((item = this->listView->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != -1)
    {
        this->listView->items[item].ReturnBook();
    }

    this->listView->Clear();
    
    wxMessageBox("Books returned! Thank you");
    this->listView->Enable();
}

void ReturnTabPage::ReturnButtonUpdateUI(wxUpdateUIEvent& event)
{
    event.Enable(false);

    if (this->listView->GetSelectedItemCount() > 0)
        event.Enable(true);
}

ReturnTabPage::~ReturnTabPage()
{
}