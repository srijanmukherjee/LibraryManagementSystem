#pragma once
#include "sqlite3.h"
#include "register.h"

class Model
{
protected:
    static sqlite3 *conn;
    friend void RegisterModels(sqlite3 *db);

public:
    static int CreateModel() { return 0; }
    static sqlite3_stmt* PrepareStatement(const char* sql); 
};