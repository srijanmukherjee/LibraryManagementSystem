#include <stdexcept>
#include "model.h"
#include "spdlog/spdlog.h"

sqlite3* Model::conn = 0;

sqlite3_stmt* Model::PrepareStatement(const char *sql)
{
    spdlog::debug("Executing {}", sql);
    sqlite3_stmt* stmt;

    int returnCode = sqlite3_prepare_v3(
        Model::conn,
        sql,
        -1,
        0,
        &stmt,
        NULL);

    if (returnCode != SQLITE_OK)
    {
        throw std::runtime_error("Could not prepare statement");
    }

    return stmt;
}