#include "MemberListCtrl.h"
#include "spdlog/spdlog.h"

MemberListCtrl::MemberListCtrl(
    wxWindow *parent,
    wxWindowID winid,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxValidator& validator,
    const wxString &name)
    : wxListCtrl(parent, winid, pos, size, style, validator, name)
{
    AppendColumn("ID");
    AppendColumn("Name");
    AppendColumn("Email");
    AppendColumn("Phone");
    AppendColumn("Address");
    AppendColumn("Since");
    AppendColumn("Registered?");
    SetColumnWidth(1, 250);
    SetColumnWidth(2, 250);
    SetColumnWidth(3, 250);
    SetColumnWidth(4, 250);
    SetColumnWidth(5, 250);
    SetColumnWidth(6, 100);

    Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &MemberListCtrl::OnContextMenu, this);
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

void MemberListCtrl::Insert(Member &member, bool unique)
{
    if (unique)
    {
        for (Member &item : items)
        {
            if (item.id == member.id) return;
        }
    }

    this->items.push_back(member);
    this->RefreshAfterUpdate();
}

wxString MemberListCtrl::OnGetItemText(long index, long column) const
{
    Member item = items[index];
    switch(column)
    {
        case 0: return std::to_string(item.id);
        case 1: return wxString::Format("%s %s", item.firstName, item.lastName);
        case 2: return item.email;
        case 3: return item.phonenumber;
        case 4: return item.address;
        case 5: return item.createdOn;
        case 6: return item.registered ? "Yes" : "No";
        default: return "";
    }
}

void MemberListCtrl::Clear()
{
    items.clear();
    RefreshAfterUpdate();
}

void MemberListCtrl::RefreshAfterUpdate()
{
    this->SetItemCount(items.size());
    this->Refresh();
}

void MemberListCtrl::OnContextMenu(wxListEvent &event)
{
    if (contextMenuCallback != nullptr)
    {
        contextMenuCallback(this, event);
    }
}

void MemberListCtrl::SortByColumn(int column)
{
    static auto genericCompare = [](auto i1, auto i2, bool ascending) {
        return ascending ? i1 < i2 : i1 > i2;
    };

    bool ascending = this->mSortAscending;

    std::sort(items.begin(), items.end(), [column, ascending](Member b1, Member b2) {
        switch(column)
        {
            case 0: return genericCompare(b1.id, b2.id, ascending);
            case 1: return genericCompare(wxString::Format("%s %s", b1.firstName, b1.lastName), wxString::Format("%s %s", b2.firstName, b2.lastName), ascending);
            case 2: return genericCompare(b1.email, b2.email, ascending);
            case 3: return genericCompare(b1.phonenumber, b2.phonenumber, ascending);
            case 4: return genericCompare(b1.address, b2.address, ascending);
            case 5: return genericCompare(b1.createdOn, b2.createdOn, ascending);
            case 6: return genericCompare(b1.registered, b2.registered, ascending);
            default: return false;
        }
    });
}

long MemberListCtrl::GetFirstSelectedIndex()
{
    return GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
}

long MemberListCtrl::FindIndexOfId(int id)
{
    return std::find_if(items.begin(), items.end(), [id](Member i) { return i.id == id; }) - items.begin();
}

void MemberListCtrl::SetContextMenuCallBack(std::function<void(MemberListCtrl*, wxListEvent&)> func)
{
    this->contextMenuCallback = func;
}

void MemberListCtrl::Remove(long index)
{
    if (index >= this->items.size()) return;
    this->items.erase(this->items.begin() + index);
    this->RefreshAfterUpdate();
}

MemberListCtrl::~MemberListCtrl()
{

}