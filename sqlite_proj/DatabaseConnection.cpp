#include "DatabaseConnection.h"
#include <iostream>

sqlite3* DatabaseConnection::context = nullptr;

DatabaseConnection::DatabaseConnection()
{
    if (sqlite3_open("store.sqlite", &context)) {
        std::cerr << "Error open DB: " << sqlite3_errmsg(context) << std::endl;
    }
}

DatabaseConnection::~DatabaseConnection()
{
    sqlite3_close(context);
}

sqlite3* DatabaseConnection::getContext()
{
    return context ? context : nullptr;
}
