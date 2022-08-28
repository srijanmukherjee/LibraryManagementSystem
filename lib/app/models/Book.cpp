#include <stdexcept>
#include <string>
#include "Book.h"
#include "spdlog/spdlog.h"

#define NOT_SET -1
#define CODE_NNNN_MAX 9999
#define CODE_NNNN_START 1000

size_t Book::codeSequence = 0;

Book::Book(string name, string author, string publisher, int count) 
    : id(NOT_SET), name(name), author(author), publisher(publisher), count(count)
{
    if (count < 0)
    {
        throw std::invalid_argument("count should be positive");
    }
}

Book::Book(int id, string name, string author, string publisher, string code, int count) 
    : id(id), name(name), author(author), publisher(publisher), code(code), count(count)
{
}

Book::Book() : id(NOT_SET) {}

int Book::CreateModel()
{
    const char *SQL = "CREATE TABLE IF NOT EXISTS Book (id INTEGER PRIMARY KEY,"
                      "name VARCHAR(150) NOT NULL,"
                      "author VARCHAR(100) NOT NULL,"
                      "publisher VARCHER(100) NOT NULL,"
                      "code VARCHAR(5) UNIQUE NOT NULL,"
                      "count INTEGER NOT NULL)";

    spdlog::debug("Creating model `Book`\nExecuting\n{}", SQL);

    int returnCode = sqlite3_exec(Model::conn, SQL, NULL, NULL, NULL);

    sqlite3_stmt* stmt = Model::PrepareStatement("SELECT id FROM Book ORDER BY id DESC LIMIT 1");

    // use the last id as the number to start the code sequence from
    // it should prevent code collision when books are deleted
    if (sqlite3_step(stmt) == SQLITE_DONE)
    {
        Book::codeSequence = 0;
    }
    else
    {
        Book::codeSequence = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    return returnCode;
}

Book Book::Load(sqlite3_stmt *stmt)
{
    int id = sqlite3_column_int(stmt, 0);
    string name((char *)sqlite3_column_text(stmt, 1));
    string author((char *)sqlite3_column_text(stmt, 2));
    string publisher((char *)sqlite3_column_text(stmt, 3));
    string code((char *)sqlite3_column_text(stmt, 4));
    int count = sqlite3_column_int(stmt, 5);
    return Book(id, name, author, publisher, code, count);
}

int Book::Save()
{
    return id == NOT_SET ? AddNew() : Update();
}

int Book::AddNew()
{
    code = Book::NextCode();
    Book::codeSequence += 1;

    sqlite3_stmt* stmt = Model::PrepareStatement(
        "INSERT INTO Book (name, author, publisher, code, count) VALUES (?, ?, ?, ?, ?)"
    );

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, NULL);
    sqlite3_bind_text(stmt, 2, author.c_str(), -1, NULL);
    sqlite3_bind_text(stmt, 3, publisher.c_str(), -1, NULL);
    sqlite3_bind_text(stmt, 4, code.c_str(), -1, NULL);
    sqlite3_bind_int(stmt, 5, count);
    int returnCode = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (returnCode == SQLITE_DONE)
    {
        id = sqlite3_last_insert_rowid(Model::conn);
    }

    return returnCode == SQLITE_DONE;
}

int Book::Update()
{
    sqlite3_stmt* stmt = Model::PrepareStatement(
        "UPDATE Book SET name=?, author=?, publisher=?, count=? WHERE id=?"
    );

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, NULL);
    sqlite3_bind_text(stmt, 2, author.c_str(), -1, NULL);
    sqlite3_bind_text(stmt, 3, publisher.c_str(), -1, NULL);
    sqlite3_bind_int(stmt, 4, count);
    sqlite3_bind_int(stmt, 5, id);

    int returnCode = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    return returnCode == SQLITE_DONE;
}

int Book::Delete()
{
    if (id == NOT_SET)
    {
        throw std::runtime_error("Object not saved");
    }

    sqlite3_stmt* stmt = Model::PrepareStatement(
        "DELETE FROM Book WHERE id=?"
    );

    sqlite3_bind_int(stmt, 1, id);
    int returnCode = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return returnCode == SQLITE_DONE;
}

string Book::NextCode()
{
    int n = Book::codeSequence;
    if (n > CODE_NNNN_MAX - CODE_NNNN_START)
    {
        n %= (CODE_NNNN_MAX - CODE_NNNN_START + 1);
    }

    string alpha;
    string numeric = std::to_string(CODE_NNNN_START + n);

    int k = Book::codeSequence / (CODE_NNNN_MAX - CODE_NNNN_START + 1);

    if (k == 0)
    {
        alpha = 'A';
    }

    while (k)
    {
        alpha = (char)('A' + k % 26) + alpha;
        k /= 26;
    }

    return alpha + numeric;
}

Book Book::FindByCode(const char *code)
{
    const char *SQL = "SELECT * FROM Book WHERE code = ?;";

    sqlite3_stmt *stmt = Model::PrepareStatement(SQL);

    sqlite3_bind_text(stmt, 1, code, -1, NULL);
    int result = sqlite3_step(stmt);
    Book book;
    if (result == SQLITE_ROW) book = Book::Load(stmt);
    sqlite3_finalize(stmt);

    if (book.id == NOT_SET)
    {
        throw std::runtime_error("Could not find book");
    }

    return book;
}

Book Book::FindById(int id)
{
    sqlite3_stmt *stmt = Model::PrepareStatement("SELECT * FROM Book WHERE id = ?");
    sqlite3_bind_int(stmt, 1, id);
    int result = sqlite3_step(stmt);
    Book book;
    if (result == SQLITE_ROW) book = Book::Load(stmt);
    sqlite3_finalize(stmt);

    if (book.id == NOT_SET)
    {
        throw std::runtime_error("Could not find book");
    }

    return book;
}

vector<Book> Book::FindByName(string name)
{
    return Book::FindByParameter("name", name);
}

vector<Book> Book::FindByAuthor(string author)
{
    return Book::FindByParameter("author", author);
}

vector<Book> Book::FindByPublisher(string publisher)
{
    return Book::FindByParameter("publisher", publisher);
}

vector<Book> Book::GetAll()
{
    string SQL = "SELECT * FROM Book";

    sqlite3_stmt *stmt = Model::PrepareStatement(SQL.c_str());

    vector<Book> result;

    while (sqlite3_step(stmt) != SQLITE_DONE)
    {
        result.push_back(Book::Load(stmt));
    }

    sqlite3_finalize(stmt);
    return result;
}


vector<Book> Book::Find(string keyword)
{
    string SQL = "SELECT * FROM Book WHERE name LIKE ?1 OR author LIKE ?1 OR publisher LIKE ?1 OR code LIKE ?1;";

    sqlite3_stmt *stmt = Model::PrepareStatement(SQL.c_str());
    string searchKeyword = "%" + keyword + "%";
    sqlite3_bind_text(stmt, 1, searchKeyword.c_str(), -1, NULL);

    vector<Book> result;

    while (sqlite3_step(stmt) != SQLITE_DONE)
    {
        result.push_back(Book::Load(stmt));
    }

    sqlite3_finalize(stmt);
    return result;
}

vector<Book> Book::FindByParameter(string parameter, string keyword)
{
    string SQL = "SELECT * FROM Book WHERE " + parameter + " LIKE ?;";

    sqlite3_stmt *stmt = Model::PrepareStatement(SQL.c_str());
    string searchKeyword = "%" + keyword + "%";
    sqlite3_bind_text(stmt, 1, searchKeyword.c_str(), -1, NULL);

    vector<Book> result;

    while (sqlite3_step(stmt) != SQLITE_DONE)
    {
        result.push_back(Book::Load(stmt));
    }

    sqlite3_finalize(stmt);
    return result;
}