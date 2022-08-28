#include "BookListCtrl.h"
#include "spdlog/spdlog.h"

BookListCtrl::BookListCtrl(
    wxWindow *parent,
    wxWindowID winid,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxValidator& validator,
    const wxString &name)
    : wxListCtrl(parent, winid, pos, size, style, validator, name)
{
    AppendColumn("Code");
    AppendColumn("Name");
    AppendColumn("Author");
    AppendColumn("Publisher");
    AppendColumn("Quantity");
    AppendColumn("Available");
    SetColumnWidth(1, 250);
    SetColumnWidth(2, 250);
    SetColumnWidth(3, 250);

    Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &BookListCtrl::OnContextMenu, this);
    Bind(wxEVT_LIST_COL_CLICK, [this](wxListEvent &event) {
        auto selectedIndex = this->GetFirstSelectedIndex();
        std::string selectedCode;

        if (selectedIndex != -1)
        {
            selectedCode = this->items[selectedIndex].code;
            this->SetItemState(selectedIndex, 0, wxLIST_STATE_SELECTED);
        }


        this->SortByColumn(event.GetColumn());
        this->RefreshAfterUpdate();
        
        if (selectedIndex != -1)
        {
            auto indexToSelect = this->FindIndexOfCode(selectedCode);
            this->SetItemState(indexToSelect, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
            this->EnsureVisible(indexToSelect);
        }

        this->mSortAscending = !this->mSortAscending;
    });
}

void BookListCtrl::Insert(Book &book, bool unique)
{
    if (unique)
    {
        for (BookItem &item : items)
        {
            if (item.code == book.code) return;
        }
    }

    this->items.push_back({ book.id, book.count, CountAvailableQuantity(book), book.code, book.name, book.author, book.publisher });
    this->RefreshAfterUpdate();
}

wxString BookListCtrl::OnGetItemText(long index, long column) const
{
    BookItem item = items[index];
    switch(column)
    {
        case 0: return item.code;
        case 1: return item.name;
        case 2: return item.author;
        case 3: return item.publisher;
        case 4: return std::to_string(item.count);
        case 5: return std::to_string(item.available);
        default: return "";
    }
}

void BookListCtrl::Clear()
{
    items.clear();
    RefreshAfterUpdate();
}

void BookListCtrl::RefreshAfterUpdate()
{
    this->SetItemCount(items.size());
    this->Refresh();
}

void BookListCtrl::OnContextMenu(wxListEvent &event)
{
    if (contextMenuCallback != nullptr)
    {
        contextMenuCallback(this, event);
    }
}

void BookListCtrl::SortByColumn(int column)
{
    static auto genericCompare = [](auto i1, auto i2, bool ascending) {
        return ascending ? i1 < i2 : i1 > i2;
    };

    bool ascending = this->mSortAscending;

    std::sort(items.begin(), items.end(), [column, ascending](BookItem b1, BookItem b2) {
        switch(column)
        {
            case 0: return genericCompare(b1.code, b2.code, ascending);
            case 1: return genericCompare(b1.name, b2.name, ascending);
            case 2: return genericCompare(b1.author, b2.author, ascending);
            case 3: return genericCompare(b1.publisher, b2.publisher, ascending);
            case 4: return genericCompare(b1.count, b2.count, ascending);
            case 5: return genericCompare(b1.available, b2.available, ascending);
            default: return false;
        }
    });
}

long BookListCtrl::GetFirstSelectedIndex()
{
    return GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
}

long BookListCtrl::FindIndexOfCode(std::string code)
{
    return std::find_if(items.begin(), items.end(), [code](BookItem i) { return i.code == code; }) - items.begin();
}

void BookListCtrl::SetContextMenuCallBack(std::function<void(BookListCtrl*, wxListEvent&)> func)
{
    this->contextMenuCallback = func;
}

void BookListCtrl::Remove(long index)
{
    if (index >= this->items.size()) return;
    this->items.erase(this->items.begin() + index);
    this->RefreshAfterUpdate();
}

BookListCtrl::~BookListCtrl()
{
}