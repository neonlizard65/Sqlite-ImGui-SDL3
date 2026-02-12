#pragma once
#include <sqlite3.h>

class DatabaseConnection
{
private:
	static sqlite3* context;
public:
	DatabaseConnection();
	~DatabaseConnection();

	static sqlite3* getContext();
};

