#include "Database.h"
#include "spdlog/spdlog.h"
#include "../constants.h"

void LoadDatabaseOrExit(const char *filename, sqlite3 **db)
{
    if (sqlite3_open(filename, db) == SQLITE_OK)
    {
        spdlog::info("Opened database `{}`", filename);
    }
    else
    {
        spdlog::critical("Failed to open database `{}`", filename);
        exit(EXIT_DB_ERROR);
    }
}