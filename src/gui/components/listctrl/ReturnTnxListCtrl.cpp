#include "ReturnTnxListCtrl.h"
#include "spdlog/spdlog.h"

ReturnTnxListCtrl::ReturnTnxListCtrl(
    wxWindow *parent,
    wxWindowID winid,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxValidator& validator,
    const wxString &name)
    : wxListCtrl(parent, winid, pos, size, style, validator, name)
{
    AppendColumn("Tnx no.");
    AppendColumn("Code");
    AppendColumn("Name");
    AppendColumn("Author");
    AppendColumn("Publisher");
    AppendColumn("Issued on");
    AppendColumn("Issued for");
    SetColumnWidth(1, 100);
    SetColumnWidth(2, 250);
    SetColumnWidth(3, 250);
    SetColumnWidth(4, 250);
    SetColumnWidth(5, 250);
    SetColumnWidth(6, 100);

    Bind(wxEVT_LIST_COL_CLICK, [this](wxListEvent &event) {
        auto selectedIndex = this->GetFirstSelectedIndex();
        int selectedId;

        if (selectedIndex != -1)
        {
            selectedId = this->items[selectedIndex].id;
            this->SetItemState(selectedIndex, 0, wxLIST_STATE_SELECTED);
        }

        this->SortByColumn(event.GetColumn());
        this->RefreshAfterUpdate();
        
        if (selectedIndex != -1)
        {
            auto indexToSelect = this->FindIndexOfId(selectedId);
            this->SetItemState(indexToSelect, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
            this->EnsureVisible(indexToSelect);
        }

        this->mSortAscending = !this->mSortAscending;
    });
}

void ReturnTnxListCtrl::Insert(History &history, bool unique)
{
    if (unique)
    {
        for (History &item : items)
        {
            if (item.id == history.id) return;
        }
    }

    this->items.push_back(history);
    this->RefreshAfterUpdate();
}

wxString ReturnTnxListCtrl::OnGetItemText(long index, long column) const
{
    History item = items[index];
    switch(column)
    {
        case 0: return std::to_string(item.id);
        case 1: return item.book.code;
        case 2: return item.book.name;
        case 3: return item.book.author;
        case 4: return item.book.publisher;
        case 5: return item.issueDate;
        case 6: return std::to_string(item.issuedForDays);
        default: return "";
    }
}

void ReturnTnxListCtrl::Clear()
{
    items.clear();
    RefreshAfterUpdate();
}

void ReturnTnxListCtrl::RefreshAfterUpdate()
{
    this->SetItemCount(items.size());
    this->Refresh();
}

void ReturnTnxListCtrl::SortByColumn(int column)
{
    static auto genericCompare = [](auto i1, auto i2, bool ascending) {
        return ascending ? i1 < i2 : i1 > i2;
    };

    bool ascending = this->mSortAscending;

    std::sort(items.begin(), items.end(), [column, ascending](History b1, History b2) {
        switch(column)
        {
            case 0: return genericCompare(b1.id, b2.id, ascending);
            case 1: return genericCompare(b1.book.code, b2.book.code, ascending);
            case 2: return genericCompare(b1.book.name, b2.book.name, ascending);
            case 3: return genericCompare(b1.book.author, b2.book.author, ascending);
            case 4: return genericCompare(b1.book.publisher, b2.book.publisher, ascending);
            case 5: return genericCompare(b1.issueDate, b2.issueDate, ascending);
            case 6: return genericCompare(b1.issuedForDays, b2.issuedForDays, ascending);
            default: return false;
        }
    });
}

long ReturnTnxListCtrl::GetFirstSelectedIndex()
{
    return GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
}

long ReturnTnxListCtrl::FindIndexOfId(int id)
{
    return std::find_if(items.begin(), items.end(), [id](History i) { return i.id == id; }) - items.begin();
}

void ReturnTnxListCtrl::Remove(long index)
{
    if (index >= this->items.size()) return;
    this->items.erase(this->items.begin() + index);
    this->RefreshAfterUpdate();
}

ReturnTnxListCtrl::~ReturnTnxListCtrl()
{
}