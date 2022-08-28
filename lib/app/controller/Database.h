#pragma once
#include "sqlite3.h"

void LoadDatabaseOrExit(const char *filename, sqlite3 **db);