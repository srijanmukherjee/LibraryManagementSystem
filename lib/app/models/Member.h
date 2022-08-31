#pragma once
#include <string>
#include <vector>
#include "model.h"
#include "sqlite3.h"

using std::string;
using std::vector;

class Member : public Model
{
public:
    int id;
    string firstName;
    string lastName;
    string createdOn;
    string address;
    string phonenumber;
    string email;
    bool registered;

    Member();
    Member(const Member &member);
    Member(string firstName, string lastName, string address, string phonenumber, string email);
    
    int Delete();
    int Save();
    int Unregister();

    static Member FindById(int id);
    static vector<Member> Find(string keyword);
    static vector<Member> FindByPhoneNumber(string phonenumber);
    static vector<Member> FindByEmail(string email);
    static vector<Member> GetAll();
    static int CreateModel();

private:
    Member(int id, string firstname, string lastName, string address, string phonenumber, string email, string createdOn, bool registered);
    int AddNew();
    int Update();
    static Member Load(sqlite3_stmt* stmt);
    static vector<Member> FindByParameter(string parameter, string keyword);
};
