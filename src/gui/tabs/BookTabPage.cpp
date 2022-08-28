#include "BookTabPage.h"
#include "../../gui.h"

#include <vector>
#include <thread>
#include "../mainwindow.h"

using std::vector;

BookTabPage::BookTabPage(
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
    listView = new BookListCtrl(this, wxID_ANY);
    listView->SetContextMenuCallBack([this](BookListCtrl* list, wxListEvent& event) {
        this->OnBookListContextMenu(list, event);
    });

    // controls
    wxBoxSizer *controlSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *allButton = new wxButton(this, wxID_ANY, _("All Books"));
    wxButton *refreshButton = new wxButton(this, wxID_ANY, _("Refresh"));
    allButton->Bind(wxEVT_BUTTON, &BookTabPage::ShowAllBooks, this);
    refreshButton->Bind(wxEVT_BUTTON, &BookTabPage::Refresh, this);
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

void BookTabPage::PopulateList()
{
    this->listView->Clear();
    this->UpdateSearchState(true);
    try {
        vector<Book> books = Book::GetAll();
        for (Book& book : books)
        {
            this->listView->Insert(book);
        }
        messageSizer->Show(false);
        listView->Show();
    } catch(std::exception &e) {
        wxMessageBox("Could not load books! Contact developer");   
    }
}

void BookTabPage::Search(wxString code, wxString name, wxString author, wxString publisher)
{
    listView->Show(false);
    messageText->SetLabel(_("Loading..."));
    messageSizer->Show(true);
    Fit();

    const auto f = [this, code, name, author, publisher]() {

        vector<Book> books;
        this->UpdateSearchState(false, code, name, author, publisher);

        if (code != wxEmptyString)
        {
            try
            {
                books.push_back(Book::FindByCode(code.c_str()));
            }
            catch(const std::exception& e)
            {
                spdlog::error(e.what());
            }
        }
        else if (name != wxEmptyString)
        {
            books = Book::FindByName(name.ToStdString());
        }
        else if (author != wxEmptyString)
        {
            books = Book::FindByAuthor(author.ToStdString());
        }
        else if (publisher != wxEmptyString)
        {
            books = Book::FindByPublisher(publisher.ToStdString());
        }
        
        if (books.size() == 0)
        {
            messageText->SetLabel(_("Could not find book"));
        }
        else
        {
            wxGetApp().CallAfter([this, books](){
                this->listView->Clear();
                for (Book book : books)
                {
                    this->listView->Insert(book);
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

void BookTabPage::ShowAllBooks(wxCommandEvent& event)
{
    UpdateSearchState(true);
    listView->Show(false);
    messageText->SetLabel(_("Loading..."));
    messageSizer->Show(true);
    Fit();

    const auto f = [this]() {
        try {
            vector<Book> books = Book::GetAll();
            wxGetApp().CallAfter([this, books](){
                this->listView->Clear();
                for (Book book : books)
                {
                    this->listView->Insert(book);
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

void BookTabPage::Reload()
{
    if (mLastSearchState.all)
    {
        this->PopulateList();
    }
    else
    {
        this->Search(mLastSearchState.code, mLastSearchState.name, mLastSearchState.author, mLastSearchState.publisher);
    }
}

void BookTabPage::Refresh(wxCommandEvent &event)
{
    this->Reload();
}

void BookTabPage::UpdateSearchState(bool all, wxString code, wxString name, wxString author, wxString publisher)
{
    this->mLastSearchState.all = all;   
    this->mLastSearchState.code = code;   
    this->mLastSearchState.name = name;   
    this->mLastSearchState.author = author;   
    this->mLastSearchState.publisher = publisher;   
}

void BookTabPage::OnBookListContextMenu(BookListCtrl* list, wxListEvent& event)
{
    wxMenu menu;
    wxMenuItem *lendItem = menu.Append(wxID_ANY, _("Lend"));
    menu.Bind(wxEVT_MENU, [this, event, list](wxCommandEvent&){
        this->LendBookCallback(list->items[event.GetIndex()].code);
    }, lendItem->GetId());
    
    wxMenuItem *editItem = menu.Append(wxID_ANY, _("Edit"));
    menu.Bind(wxEVT_MENU, [this, event, list](wxCommandEvent&){
        this->EditBookCallback(list->items[event.GetIndex()].code);
    }, editItem->GetId());

    wxMenuItem *deleteItem = menu.Append(wxID_ANY, _("Delete"));
    menu.Bind(wxEVT_MENU, [this, event, list](wxCommandEvent&){
        this->DeleteBookCallback(list->items[event.GetIndex()].code);
    }, deleteItem->GetId());
    
    list->PopupMenu(&menu, event.GetPoint());
}

void BookTabPage::SetLendBookCallback(std::function<void(std::string)> func)
{
    LendBookCallback = func;
}

void BookTabPage::SetDeleteBookCallback(std::function<void(std::string)> func)
{
    DeleteBookCallback = func;
}

void BookTabPage::SetEditBookCallback(std::function<void(std::string)> func)
{
    EditBookCallback = func;
}

BookTabPage::~BookTabPage()
{
}