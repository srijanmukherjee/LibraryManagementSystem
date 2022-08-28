#include "register.h"
#include "Book.h"
#include "Member.h"
#include "History.h"

void RegisterModels(sqlite3 *db)
{
    Model::conn = db;
    Book::CreateModel();
    Member::CreateModel();
    History::CreateModel();
}