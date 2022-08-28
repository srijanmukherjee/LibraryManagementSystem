#include "Member.h"
#include "spdlog/spdlog.h"

#define NOT_SET -1

Member::Member(string firstName, string lastName, string address, string phonenumber, string email)
    : id(NOT_SET), firstName(firstName), lastName(lastName), address(address), phonenumber(phonenumber), email(email)
{
    // TODO: verify the fields
}

Member::Member(int id, string firstname, string lastname, string address, string phonenumber, string email, string createdOn)
    : id(id), firstName(firstname), lastName(lastname), address(address), phonenumber(phonenumber), email(email), createdOn(createdOn)
{
}

Member::Member() : id(NOT_SET) {}

Member::Member(const Member &member) : id(member.id), firstName(member.firstName), lastName(member.lastName), address(member.address), phonenumber(member.phonenumber), email(member.email), createdOn(member.createdOn)
{
    
}

int Member::CreateModel()
{
    const char *SQL = "CREATE TABLE IF NOT EXISTS Member (id INTEGER PRIMARY KEY,"
                      "firstName VARCHAR(30) NOT NULL,"
                      "lastName VARCHAR(30) NOT NULL,"
                      "address VARCHAR(200) NOT NULL,"
                      "phonenumber VARCHER(13) UNIQUE NOT NULL,"
                      "email VARCHAR(100) UNIQUE NOT NULL,"
                      "createdOn TIMESTAMP DEFAULT CURRENT_TIMESTAMP)";

    spdlog::debug("Creating model `Member`\nExecuting\n{}", SQL);

    return sqlite3_exec(Model::conn, SQL, NULL, NULL, NULL);
}


Member Member::Load(sqlite3_stmt *stmt)
{
    int id = sqlite3_column_int(stmt, 0);
    string firstName((char*)sqlite3_column_text(stmt, 1));
    string lastName((char*)sqlite3_column_text(stmt, 2));
    string address((char*)sqlite3_column_text(stmt, 3));
    string phonenumber((char*)sqlite3_column_text(stmt, 4));
    string email((char*)sqlite3_column_text(stmt, 5));
    string createdOn((char*)sqlite3_column_text(stmt, 6));
    return Member(id, firstName, lastName, address, phonenumber, email, createdOn);
}

int Member::Save()
{
    return id == NOT_SET ? AddNew() : Update();
}

int Member::AddNew()
{
    sqlite3_stmt* stmt = Model::PrepareStatement(
        "INSERT INTO Member (firstName, lastName, address, phonenumber, email) VALUES (?, ?, ?, ?, ?)"
    );

    sqlite3_bind_text(stmt, 1, firstName.c_str(), -1, NULL);
    sqlite3_bind_text(stmt, 2, lastName.c_str(), -1, NULL);
    sqlite3_bind_text(stmt, 3, address.c_str(), -1, NULL);
    sqlite3_bind_text(stmt, 4, phonenumber.c_str(), -1, NULL);
    sqlite3_bind_text(stmt, 5, email.c_str(), -1, NULL);
    int returnCode = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (returnCode == SQLITE_DONE)
    {
        id = sqlite3_last_insert_rowid(Model::conn);
    }

    return returnCode == SQLITE_DONE;
}

int Member::Update()
{
    sqlite3_stmt* stmt = Model::PrepareStatement(
        "UPDATE Member SET firstName=?, lastName=?, address=?, phonenumber=?, email=? WHERE id=?"
    );

    sqlite3_bind_text(stmt, 1, firstName.c_str(), -1, NULL);
    sqlite3_bind_text(stmt, 2, lastName.c_str(), -1, NULL);
    sqlite3_bind_text(stmt, 3, address.c_str(), -1, NULL);
    sqlite3_bind_text(stmt, 4, phonenumber.c_str(), -1, NULL);
    sqlite3_bind_text(stmt, 5, email.c_str(), -1, NULL);
    sqlite3_bind_int(stmt, 6, id);

    int returnCode = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    return returnCode == SQLITE_DONE;
}

int Member::Delete()
{
    if (id == NOT_SET)
    {
        throw std::runtime_error("Object not saved");
    }

    sqlite3_stmt* stmt = Model::PrepareStatement(
        "DELETE FROM Member WHERE id=?"
    );

    sqlite3_bind_int(stmt, 1, id);
    int returnCode = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return returnCode == SQLITE_DONE;
}

Member Member::FindById(int id)
{
    sqlite3_stmt* stmt = Model::PrepareStatement(
        "SELECT * FROM Member WHERE id=?"
    );

    sqlite3_bind_int(stmt, 1, id);
    int result = sqlite3_step(stmt);
    Member member;
    if (result == SQLITE_ROW) member = Member::Load(stmt);
    sqlite3_finalize(stmt);

    if (member.id == NOT_SET)
    {
        throw std::runtime_error("Member does not exist");
    }

    return member;
}

vector<Member> Member::FindByPhoneNumber(string phonenumber)
{
    return Member::FindByParameter("phonenumber", phonenumber);
}

vector<Member> Member::FindByEmail(string email)
{
    return Member::FindByParameter("email", email);
}

vector<Member> Member::Find(string keyword)
{
    string SQL = "SELECT * FROM Member WHERE phonenumber LIKE ?1 OR email LIKE ?1 OR firstName LIKE ?1 OR lastName LIKE ?1 OR address LIKE ?1;";

    sqlite3_stmt *stmt = Model::PrepareStatement(SQL.c_str());
    string searchKeyword = "%" + keyword + "%";
    sqlite3_bind_text(stmt, 1, searchKeyword.c_str(), -1, NULL);

    vector<Member> result;

    while (sqlite3_step(stmt) != SQLITE_DONE)
    {
        result.push_back(Member::Load(stmt));
    }

    sqlite3_finalize(stmt);
    return result;
}

vector<Member> Member::GetAll()
{
    string SQL = "SELECT * FROM Member";
    sqlite3_stmt *stmt = Model::PrepareStatement(SQL.c_str());
    vector<Member> result;

    while (sqlite3_step(stmt) != SQLITE_DONE)
    {
        result.push_back(Member::Load(stmt));
    }

    sqlite3_finalize(stmt);
    return result;
}

vector<Member> Member::FindByParameter(string parameter, string keyword)
{
    string SQL = "SELECT * FROM Member WHERE " + parameter + " LIKE ?;";

    sqlite3_stmt *stmt = Model::PrepareStatement(SQL.c_str());
    string searchKeyword = "%" + keyword + "%";
    sqlite3_bind_text(stmt, 1, searchKeyword.c_str(), -1, NULL);

    vector<Member> result;

    while (sqlite3_step(stmt) != SQLITE_DONE)
    {
        result.push_back(Member::Load(stmt));
    }

    sqlite3_finalize(stmt);
    return result;
}
