#include "DatabaseConnection.h"
#include <iostream>

sqlite3* DatabaseConnection::context = nullptr;

void DatabaseConnection::initContext()
{
    if (sqlite3_open("store.db", &context) != SQLITE_OK) {
        std::cerr << "Error open DB: " << sqlite3_errmsg(context) << std::endl;
        sqlite3_close(context);
    }
}

void DatabaseConnection::clearContext()
{
    sqlite3_close(context);
    context = nullptr;
}

sqlite3* DatabaseConnection::getContext()
{
    if (context == nullptr) {
        initContext();
    }
    return context;
}
