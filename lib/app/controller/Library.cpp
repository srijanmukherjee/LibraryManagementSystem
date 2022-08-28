#include "Library.h"


int CountAvailableQuantity(Book &book)
{
    return book.count - History::CountNotReturned(book);
}

void LendBook(Book &book, Member &member, int issueFor)
{
    History hist(book, member, issueFor);
    hist.Save();
}