#include "LendTabPage.h"

#include <wx/valnum.h>
#include <wx/valtext.h>

#include "../../gui.h"
#include "../id.h"

#include <vector>
#include <thread>

using std::vector;

LendTabPage::LendTabPage(
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
    memberIdCtrl->Bind(wxEVT_TEXT_ENTER, &LendTabPage::OnSearchMember, this);
    memberSearchBtn = new wxButton(this, wxID_ANY, _("Search"));
    memberSearchBtn->Bind(wxEVT_BUTTON, &LendTabPage::OnSearchMember, this);
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
    wxStaticText *bookListTitle = new wxStaticText(this, wxID_ANY, _("Add Book"));
    bookListTitle->SetFont(bookListTitle->GetFont().Scale(1.2));
    bookListSizer->Add(bookListTitle, 0, wxALL | wxEXPAND, 10);
    // book code input
    wxBoxSizer *bookCodeSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *bookCodeText = new wxStaticText(this, wxID_ANY, _("Book Code"));
    bookCodeCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxTE_CAPITALIZE);
    bookCodeCtrl->SetValidator(wxTextValidator(wxFILTER_ALPHANUMERIC | wxFILTER_EMPTY));
    bookCodeCtrl->Bind(wxEVT_TEXT_ENTER, &LendTabPage::OnBookAdd, this);
    bookAddBtn = new wxButton(this, wxID_ANY, _("Add"));
    bookAddBtn->Bind(wxEVT_BUTTON, &LendTabPage::OnBookAdd, this);
    bookCodeSizer->Add(bookCodeText, 0, wxCENTER | wxALL, 5);
    bookCodeSizer->Add(bookCodeCtrl, 1, wxEXPAND | wxCENTER | wxALL, 5);
    bookCodeSizer->Add(bookAddBtn, 0, wxCENTER | wxALL, 5);
    bookListSizer->Add(bookCodeSizer, 0, wxEXPAND | wxALL, 5);
    
    listView = new BookListCtrl(this, wxID_ANY);
    listView->SetContextMenuCallBack(LendTabPage::OnBookListContextMenu);
    bookListSizer->Add(listView, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(bookListSizer, 1, wxEXPAND);
    bookListSizer->Show(false);

    // controls
    wxBoxSizer *controlSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *resetButton = new wxButton(this, wxID_ANY, _("Reset"));
    resetButton->Bind(wxEVT_BUTTON, &LendTabPage::OnReset, this);

    this->issueButton = new wxButton(this, wxID_ANY, _("Issue books"));
    this->issueButton->Bind(wxEVT_BUTTON, &LendTabPage::OnIssueBook, this);
    this->issueButton->Bind(wxEVT_UPDATE_UI, &LendTabPage::LendButtonUpdateUI, this);
    this->issueButton->Show(false);
    controlSizer->Add(resetButton, 0, wxALL, 5);
    controlSizer->Add(this->issueButton, 0, wxALL, 5);
    mainSizer->Add(controlSizer, 0, wxEXPAND);
    SetSizer(mainSizer);
    Fit();
}

void LendTabPage::OnSearchMember(wxCommandEvent& event)
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

void LendTabPage::SearchMember(int id)
{
    try
    {
        Member _member = Member::FindById(id);
        if (!_member.registered)
        {
            wxMessageBox("Member is not registered\n");
            return;
        }
        
        if (this->member != nullptr)
        {
            delete this->member;
        }
        this->member = new Member(_member);
        this->DisplayMemberInfo();
        this->DisplayBookList();
        this->issueButton->Show(true);
    }
    catch(const std::exception& e)
    {
        wxMessageBox("Member not found");
    }
    
    // std::thread bck;
    // bck.detach();
}

void LendTabPage::DisplayMemberInfo(bool show)
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

void LendTabPage::DisplayBookList(bool show)
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

void LendTabPage::OnBookAdd(wxCommandEvent& event)
{
    wxString code = bookCodeCtrl->GetValue();

    if (code == wxEmptyString)
    {
        return;
    }

    this->AddBook(code.ToStdString());
}

void LendTabPage::AddBook(std::string code)
{
    try
    {
        Book book = Book::FindByCode(code.c_str());

        if (CountAvailableQuantity(book) == 0)
        {
            wxMessageBox("book not available");
            return;
        }

        this->listView->Insert(book, true);
        bookCodeCtrl->SetValue(wxEmptyString);
    }
    catch(const std::exception& e)
    {
        wxMessageBox("Book not found");
    }
    
}

void LendTabPage::Reset()
{
    if (member != nullptr)
    {
        delete member;
        member = nullptr;
    }

    this->memberInfoSizer->Show(false);
    this->bookListSizer->Show(false);
    this->issueButton->Show(false);
    this->memberIdCtrl->SetValue(wxEmptyString);
    this->listView->Clear();
    this->Fit();
}

void LendTabPage::OnReset(wxCommandEvent& event)
{
    this->Reset();
}

void LendTabPage::OnIssueBook(wxCommandEvent& event)
{
    Member memberObj(*member);
    for (BookItem& item : this->listView->items)
    {
        try
        {
            Book book = Book::FindByCode(item.code.c_str());
            LendBook(book, memberObj, 7);
        } catch(std::exception &e)
        {
            spdlog::error("Could not issue book %s", item.name);
        }
    }

    this->Reset();
    wxMessageBox("Books issued for 7 days");
}

void LendTabPage::LendButtonUpdateUI(wxUpdateUIEvent& event)
{
    event.Enable(false);

    if (this->listView->items.size() > 0)
        event.Enable(true);
}

void LendTabPage::OnBookListContextMenu(BookListCtrl* list, wxListEvent& event)
{
    long index = event.GetIndex();
    wxMenu menu;
    wxMenuItem *removeItem = menu.Append(wxID_ANY, _("Remove"));
    menu.Bind(wxEVT_MENU, [index, list](wxCommandEvent& evt) {
        list->Remove(index);
    }, removeItem->GetId());
    list->PopupMenu(&menu, event.GetPoint());
}


LendTabPage::~LendTabPage()
{
}