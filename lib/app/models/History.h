#pragma once
#include <string>
#include <vector>
#include "model.h"
#include "Member.h"
#include "Book.h"
#include "sqlite3.h"

using std::string;
using std::vector;

#define BOOK_NOT_RETURNED 1
#define BOOK_RETURNED 2

class History : public Model
{
public:
    int id;
    Book book;
    Member member;
    string issueDate;
    string returnDate;
    int issuedForDays;
    bool returned;
    
    History();
    History(Book book, Member member, int issuedForDays);

    int Save();
    int Delete();
    int ReturnBook();
    static int CreateModel();
    static History GetById(int id);
    static vector<History> GetHistoryOfMember(Member &member, int flag = 0);
    static vector<History> GetHistoryOfBook(Book &book);
    static vector<History> GetAll();
    static int CountNotReturned(Book &book);

private:
    History(int id, Book book, Member member, string issueDate, string returnDate, int issuedForDays, bool returned);
    static History Load(sqlite3_stmt* stmt);
    int AddNew();
    int Update();
};