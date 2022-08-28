#include "MemberTabPage.h"
#include "../../gui.h"

#include <vector>
#include <thread>

using std::vector;

MemberTabPage::MemberTabPage(
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

    // message
    messageText = new wxStaticText(this, wxID_ANY, _("Loading..."));
    messageSizer->Add(messageText, 1, wxCENTER);

    // list view
    listView = new MemberListCtrl(this, wxID_ANY);

    // controls
    wxBoxSizer *controlSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *allButton = new wxButton(this, wxID_ANY, _("All Members"));
    wxButton *refreshButton = new wxButton(this, wxID_ANY, _("Refresh"));
    allButton->Bind(wxEVT_BUTTON, &MemberTabPage::ShowAllMembers, this);
    refreshButton->Bind(wxEVT_BUTTON, &MemberTabPage::Refresh, this);
    controlSizer->Add(allButton, 0, wxALL, 5);
    controlSizer->Add(refreshButton, 0, wxTOP | wxBOTTOM | wxRIGHT, 5);

    mainSizer->Add(messageSizer, 1, wxCENTER);
    mainSizer->Add(listView, 1, wxEXPAND);
    mainSizer->Add(controlSizer, 0, wxEXPAND);

    listView->Show(false);

    PopulateList();
    SetSizer(mainSizer);
    Fit();
}

void MemberTabPage::PopulateList()
{
    this->listView->Clear();
    this->UpdateSearchState(true);
    try {
        vector<Member> members = Member::GetAll();
        for (Member& member : members)
        {
            this->listView->Insert(member);
        }
        messageSizer->Show(false);
        listView->Show();
    } catch(std::exception &e) {
        wxMessageBox("Could not load members! Contact developer");   
    }
}

void MemberTabPage::Search(wxString name, wxString email, wxString phonenumber, wxString address)
{
    listView->Show(false);
    messageText->SetLabel(_("Loading..."));
    messageSizer->Show(true);
    Fit();

    const auto f = [this, name, email, phonenumber, address]() {

        vector<Member> members;
        this->UpdateSearchState(false, name, email, phonenumber, address);
        if (name != wxEmptyString)
        {
            members = Member::Find(name.ToStdString());
        }
        else if (email != wxEmptyString)
        {
            members = Member::Find(email.ToStdString());
        }
        else if (phonenumber != wxEmptyString)
        {
            members = Member::Find(phonenumber.ToStdString());
        }
        else if(address != wxEmptyString)
        {
            members = Member::Find(address.ToStdString());
        }
        
        if (members.size() == 0)
        {
            messageText->SetLabel(_("Could not find member"));
        }
        else
        {
            wxGetApp().CallAfter([this, members](){
                this->listView->Clear();
                for (Member member : members)
                {
                    this->listView->Insert(member);
                }
                this->messageSizer->Show(false);
                this->listView->Show(true);
            });
        }

        this->Fit();
    };

    std::thread bck{f};
    bck.detach();
}

void MemberTabPage::ShowAllMembers(wxCommandEvent& event)
{
    UpdateSearchState(true);
    listView->Show(false);
    messageText->SetLabel(_("Loading..."));
    messageSizer->Show(true);
    Fit();

    const auto f = [this]() {
        try {
            vector<Member> members = Member::GetAll();
            wxGetApp().CallAfter([this, members](){
                this->listView->Clear();
                for (Member member : members)
                {
                    this->listView->Insert(member);
                }
                this->messageSizer->Show(false);
                this->listView->Show(true);
            });
        } catch (std::exception& e) {
            messageText->SetLabel(_("Could not find book"));
        }

        this->Fit();
    };

    std::thread bck{f};
    bck.detach();
}

void MemberTabPage::Reload()
{
    if (mLastSearchState.all)
    {
        this->PopulateList();
    }
    else
    {
        this->Search(mLastSearchState.name, mLastSearchState.email, mLastSearchState.phonenumber, mLastSearchState.address);
    }
}

void MemberTabPage::Refresh(wxCommandEvent &event)
{
    this->Reload();
}

void MemberTabPage::UpdateSearchState(bool all, wxString name, wxString email, wxString phonenumber, wxString address)
{
    this->mLastSearchState.all = all;   
    this->mLastSearchState.name = name;   
    this->mLastSearchState.email = email;   
    this->mLastSearchState.phonenumber = phonenumber;   
    this->mLastSearchState.address = address;   
}

MemberTabPage::~MemberTabPage()
{
}