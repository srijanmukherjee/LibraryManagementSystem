#pragma once
#include <string>
#include <vector>
#include "sqlite3.h"
#include "model.h"

using std::string;
using std::vector;

class Book : public Model
{
public:
    int id;
    string name;
    string author;
    string publisher;
    string code;
    int count;

    Book();    
    Book(string name, string author, string publisher, int count);
    
    int Delete();
    int Save();
    static Book FindByCode(const char *code);
    static Book FindById(int id);
    static vector<Book> FindByName(string name);
    static vector<Book> FindByAuthor(string author);
    static vector<Book> FindByPublisher(string publisher);
    static vector<Book> GetAll();
    static vector<Book> Find(string keyword);
    static int CreateModel();

private:
    Book(int id, string name, string author, string publisher, string code, int count);
    int AddNew();
    int Update();
    static size_t codeSequence;
    static Book Load(sqlite3_stmt* stmt);
    static string NextCode();
    static vector<Book> FindByParameter(string parameter, string keyword);
};