#include "mainwindow.h"
#include <vector>
#include <wx/artprov.h>

#include "components/dialog/AddBookDialog.h"
#include "components/dialog/SearchMemberDialog.h"
#include "components/dialog/AddMemberDialog.h"
#include "SearchBookDialog.h"

#include "controller/Database.h"
#include "controller/Library.h"
#include "models/Book.h"
#include "models/register.h"
#include "spdlog/spdlog.h"
#include "AppConfig.h"
#include "../gui.h"

using std::vector;

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_IDLE(MainWindow::OnIdle)
END_EVENT_TABLE()

MainWindow::MainWindow(wxWindow *parent,
                       wxWindowID id,
                       const wxString &title,
                       const wxPoint &pos,
                       const wxSize &size,
                       long style,
                       const wxString &name) : wxFrame(parent, id, title, pos, size, style, name)
{
    LoadDatabaseOrExit(DATABASE_FILE, &db);
    RegisterModels(db);

    Bind(wxEVT_CLOSE_WINDOW, &MainWindow::OnClose, this);

    const int SIZE = 1;
    wxStatusBar *statusBar = CreateStatusBar(SIZE);
    int widths[SIZE] = {-1};
    statusBar->SetStatusWidths(SIZE, widths);

    wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *contentSizer = new wxBoxSizer(wxVERTICAL);

    tabNotebook = new wxNotebook(this, wxID_ANY);
    booksPage = new BookTabPage(tabNotebook, wxID_ANY);
    booksPage->SetLendBookCallback([this](std::string code){
        this->LendBook(code);
    });
    booksPage->SetDeleteBookCallback([this](std::string code){
        this->DeleteBook(code);
    });
    booksPage->SetEditBookCallback([this](std::string code){
        this->EditBook(code);
    });

    lendPage = new LendTabPage(tabNotebook, wxID_ANY);
    returnPage = new ReturnTabPage(tabNotebook, wxID_ANY);
    memberPage = new MemberTabPage(tabNotebook, wxID_ANY);

    tabNotebook->AddPage(booksPage, _("Books"));
    tabNotebook->AddPage(memberPage, _("Members"));
    tabNotebook->AddPage(lendPage, _("Lend"));
    tabNotebook->AddPage(returnPage, _("Return"));
    tabNotebook->Layout();
    tabNotebook->SetMinSize(wxSize(wxEXPAND, wxEXPAND));
    contentSizer->Add(tabNotebook, 1, wxEXPAND | wxALL, 0);

    /* Control Buttons */
    const int controlPanelWidth = 300;
    wxBoxSizer *controlSizer = new wxBoxSizer(wxVERTICAL);
    controlSizer->SetMinSize(wxSize(controlPanelWidth, wxEXPAND));

    // control panel
    wxPanel *controlPanel = new wxPanel(this);
    controlPanel->SetMaxSize(wxSize(controlPanelWidth, wxEXPAND));
    // controlPanel->SetBackgroundColour(wxColor(*wxWHITE));

    // control panel sizer
    wxBoxSizer *controlPanelSizer = new wxBoxSizer(wxVERTICAL);
    controlPanel->SetSizer(controlPanelSizer);
    controlPanelSizer->Fit(controlPanel);
    controlSizer->Add(controlPanel, 1, wxEXPAND);

    // heading
    wxStaticText *heading = new wxStaticText(controlPanel, wxID_ANY, _("Library Management System"));
    heading->SetFont(heading->GetFont().Scale(1.4));
    controlPanelSizer->Add(heading, 0, wxCENTER | wxBOTTOM | wxTOP, 10);

    // control buttons
    searchBookButton      = CreateControlButton(controlPanel, wxID_ANY, _("Search book"));
    searchMemberButton    = CreateControlButton(controlPanel, wxID_ANY, _("Search member"));
    lendBookButton        = CreateControlButton(controlPanel, wxID_ANY, _("Lend book"));
    returnBookButton      = CreateControlButton(controlPanel, wxID_ANY, _("Return book"));
    addBookButton         = CreateControlButton(controlPanel, wxID_ANY, _("Add book"));
    addMemberButton       = CreateControlButton(controlPanel, wxID_ANY, _("Register member"));

    searchBookButton->Bind(wxEVT_BUTTON, &MainWindow::OnSearchBook, this);
    searchMemberButton->Bind(wxEVT_BUTTON, &MainWindow::OnSearchMember, this);
    addBookButton->Bind(wxEVT_BUTTON, &MainWindow::OnAddBook, this);
    lendBookButton->Bind(wxEVT_BUTTON, &MainWindow::OnLendButtonPress, this);
    addMemberButton->Bind(wxEVT_BUTTON, &MainWindow::OnAddMember, this);
    returnBookButton->Bind(wxEVT_BUTTON, &MainWindow::OnReturnBook, this);

    mainSizer->Add(contentSizer, 1, wxEXPAND);
    mainSizer->Add(controlSizer, 0);

    SetSizer(mainSizer);
    mainSizer->Fit(this);
}

void MainWindow::OnClose(wxCloseEvent &event)
{
    if (event.CanVeto())
    {
        int answer = wxMessageBox(_("Should we close?"), _("Confirm?"), wxYES_NO);

        if (answer != wxYES)
        {
            event.Veto();
            return;
        }
    }

    Destroy();
}

void MainWindow::OnIdle(wxIdleEvent &event)
{
    SetStatusText(_("Ready!"));
}

wxButton* MainWindow::CreateControlButton(wxWindow *parent, wxWindowID id, const wxString &label)
{
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *button = new wxButton(parent, id, label);
    button->SetMinSize(wxSize(wxEXPAND, button->GetSize().y));
    button->SetToolTip(label);
    buttonSizer->Add(button, 0, wxEXPAND | wxALL, 10);
    parent->GetSizer()->Add(buttonSizer, 1, wxEXPAND);
    return button;
}

void MainWindow::OnSearchBook(wxCommandEvent& event)
{
    SearchBookDialog *dlg = new SearchBookDialog(this, wxID_ANY, _("Search Book"));

    if (dlg->ShowModal() == wxID_OK)
    {
        booksPage->Search(dlg->GetCode(), dlg->GetName(), dlg->GetAuthor(), dlg->GetPublisher());
        this->tabNotebook->SetSelection(0);
    }

    dlg->Destroy();
}

void MainWindow::OnSearchMember(wxCommandEvent& event)
{
    SearchMemberDialog *dlg = new SearchMemberDialog(this, wxID_ANY, _("Search Member"));

    if (dlg->ShowModal() == wxID_OK)
    {
        memberPage->Search(dlg->GetName(), dlg->GetEmail(), dlg->GetPhonenumber(), dlg->GetAddress());
        this->tabNotebook->SetSelection(1);
    }

    dlg->Destroy();
}

void MainWindow::CreateBook(std::string name, std::string author, std::string publisher, int count)
{
    Book book(name, author, publisher, count);
    book.Save();
    this->booksPage->Reload();
    this->tabNotebook->SetSelection(0);
}

void MainWindow::OnAddBook(wxCommandEvent& event)
{
    AddBookDialog *dlg = new AddBookDialog(this, wxID_ANY, _("Add book"));

    if (dlg->ShowModal() == wxID_OK)
    {
        int quantity;
        dlg->GetQuantity().ToInt(&quantity);
        this->CreateBook(
            dlg->GetName().ToStdString(),
            dlg->GetAuthor().ToStdString(),
            dlg->GetPublisher().ToStdString(),
            quantity
        );
    }

    dlg->Destroy();
}

void MainWindow::CreateMember(std::string fname, std::string lname, std::string email, std::string phonenumber, std::string address)
{
    Member member(fname, lname, address, phonenumber, email);
    member.Save();
    this->memberPage->Reload();
    this->tabNotebook->SetSelection(1);
}

void MainWindow::OnAddMember(wxCommandEvent& event)
{
    AddMemberDialog *dlg = new AddMemberDialog(this, wxID_ANY, _("Add member"));

    if (dlg->ShowModal() == wxID_OK)
    {
        this->CreateMember(
            dlg->GetFirstName().ToStdString(),
            dlg->GetLastName().ToStdString(),
            dlg->GetEmail().ToStdString(),
            dlg->GetPhonenumber().ToStdString(),
            dlg->GetAddress().ToStdString()
        );
    }   

    dlg->Destroy();
}

void MainWindow::OnLendButtonPress(wxCommandEvent& event)
{
    tabNotebook->SetSelection(2);
}

void MainWindow::OnReturnBook(wxCommandEvent& event)
{
    tabNotebook->SetSelection(3);
}

void MainWindow::LendBook(std::string &code)
{
    this->tabNotebook->SetSelection(2);
    this->lendPage->DisplayBookList();
    this->lendPage->AddBook(code);
}

void MainWindow::DeleteBook(std::string &code)
{
    try
    {
        Book book = Book::FindByCode(code.c_str());

        if (CountAvailableQuantity(book) != book.count)
        {
            wxMessageBox("Some books are still issued");
        }
        else
        {
            int answer = wxMessageBox(_("Delete this book?"), _("Confirm?"), wxYES_NO);

            if (answer == wxYES)
            {
                book.Delete();
                this->booksPage->Reload();   
            }
        }

    }
    catch(const std::exception& e)
    {
        spdlog::info(e.what());
    }
}

void MainWindow::EditBook(std::string &code)
{
    wxMessageBox("Not implemented");
}