#include "History.h"
#include "spdlog/spdlog.h"

#define NOT_SET -1

int History::CreateModel()
{
    const char *SQL = "CREATE TABLE IF NOT EXISTS History ("
                      "id INTEGER PRIMARY KEY,"
                      "bookId INTEGER NOT NULL,"
                      "memberId INTEGER NOT NULL,"
                      "issuedOn TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
                      "returnDate TIMESTAMP NULL DEFAULT NULL,"
                      "issuedForDays INTEGER NOT NULL,"
                      "returned INTEGER DEFAULT 0,"
                      "FOREIGN KEY (bookId) REFERENCES Book(id),"
                      "FOREIGN KEY (memberId) REFERENCES Member(id))";

    spdlog::debug("Creating model `History`\nExecuting\n{}", SQL);

    return sqlite3_exec(Model::conn, SQL, NULL, NULL, NULL);
}

History::History() 
{
}

History::History(Book book, Member member, int issuedForDays)
    : id(NOT_SET), book(book), member(member), issuedForDays(issuedForDays)
{
    returned = false;
}

History::History(int id, Book book, Member member, string issueDate, string returnDate, int issuedForDays, bool returned)
    : id(id), book(book), member(member), issueDate(issueDate), returnDate(returnDate), issuedForDays(issuedForDays), returned(returned)
{

}

int History::Save()
{
    if (id == NOT_SET) return AddNew();
    return 0;
}

int History::ReturnBook()
{
    sqlite3_stmt *stmt = Model::PrepareStatement(
        "UPDATE History SET returnDate = CURRENT_TIMESTAMP, returned = 1 WHERE id = ?"
    );

    sqlite3_bind_int(stmt, 1, id);
    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return result == SQLITE_DONE;
}

int History::Delete()
{
    if (id == NOT_SET)
    {
        throw std::runtime_error("Object not saved");
    }

    sqlite3_stmt* stmt = Model::PrepareStatement(
        "DELETE FROM History WHERE id=?"
    );

    sqlite3_bind_int(stmt, 1, id);
    int returnCode = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return returnCode == SQLITE_DONE;
}

int History::AddNew()
{
    sqlite3_stmt* stmt = Model::PrepareStatement(
        "INSERT INTO History (bookId, memberId, issuedForDays) VALUES (?, ?, ?)"
    );

    sqlite3_bind_int(stmt, 1, book.id);
    sqlite3_bind_int(stmt, 2, member.id);
    sqlite3_bind_int(stmt, 3, issuedForDays);
    int returnCode = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (returnCode == SQLITE_DONE)
    {
        id = sqlite3_last_insert_rowid(Model::conn);
    }

    return returnCode == SQLITE_DONE;
}

vector<History> History::GetAll()
{
    string SQL = "SELECT * FROM History";
    sqlite3_stmt *stmt = Model::PrepareStatement(SQL.c_str());
    vector<History> result;

    while (sqlite3_step(stmt) != SQLITE_DONE)
    {
        result.push_back(History::Load(stmt));
    }

    sqlite3_finalize(stmt);
    return result;
}

History History::GetById(int id)
{
    sqlite3_stmt* stmt = Model::PrepareStatement(
        "SELECT * FROM History WHERE id = ?"
    );

    sqlite3_bind_int(stmt, 1, id);
    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (result != SQLITE_ROW)
    {
        throw std::runtime_error("Could not find history");
    }

    return History::Load(stmt);
}

vector<History> History::GetHistoryOfBook(Book &book)
{
    string SQL = "SELECT * FROM History WHERE bookId = ? ORDER BY issuedOn ASC";
    sqlite3_stmt *stmt = Model::PrepareStatement(SQL.c_str());
    sqlite3_bind_int(stmt, 1, book.id);
    vector<History> result;

    while (sqlite3_step(stmt) != SQLITE_DONE)
    {
        result.push_back(History::Load(stmt));
    }

    sqlite3_finalize(stmt);
    return result;   
}

vector<History> History::GetHistoryOfMember(Member &member, int flag)
{
    string SQL = "SELECT * FROM History WHERE memberId = ?";

    if (flag & BOOK_NOT_RETURNED)
    {
        SQL += " AND returned = 0";
    }

    sqlite3_stmt *stmt = Model::PrepareStatement(SQL.c_str());
    sqlite3_bind_int(stmt, 1, member.id);
    vector<History> result;

    while (sqlite3_step(stmt) != SQLITE_DONE)
    {
        result.push_back(History::Load(stmt));
    }

    sqlite3_finalize(stmt);
    return result;   
}

int History::CountNotReturned(Book &book)
{
    sqlite3_stmt *stmt = Model::PrepareStatement("SELECT COUNT(*) FROM History WHERE bookId=? AND returned = 0");
    sqlite3_bind_int(stmt, 1, book.id);
    int result = sqlite3_step(stmt);
    int count = 0;
    if (result == SQLITE_ROW) count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    if (result != SQLITE_ROW) throw std::runtime_error("could not get count");
    return count;
}

History History::Load(sqlite3_stmt* stmt)
{
    int id = sqlite3_column_int(stmt, 0);
    int bookId = sqlite3_column_int(stmt, 1);
    int memberId = sqlite3_column_int(stmt, 2);
    string issuedOn((char *)sqlite3_column_text(stmt, 3));
    string returnDate;
    if (sqlite3_column_text(stmt, 4) != NULL)
    {
        returnDate = string((char *) sqlite3_column_text(stmt, 4));
    }
    int issuedForDays = sqlite3_column_int(stmt, 5);
    int returned = sqlite3_column_int(stmt, 6);

    Book book = Book::FindById(bookId);
    Member member = Member::FindById(memberId);

    return History(id, book, member, issuedOn, returnDate, issuedForDays, returned);
}