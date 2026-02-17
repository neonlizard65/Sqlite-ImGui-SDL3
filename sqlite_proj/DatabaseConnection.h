#pragma once
#include <sqlite3.h>

// DatabaseConnection::getContext() -> получить инициализированный указатель к контексту БД (nullptr если неудачное подключение)
//		Примечания: вызывается функция sqlite3_open()
// DatabaseConnection::clearContext() -> закрывает sqlite посредством команды sqlite3_close()
//		Примечания: нужно вызвать, когда сделали все запросы
class DatabaseConnection
{
private:
	static sqlite3* context;
	static void initContext();
public:
	static sqlite3* getContext();
	static void clearContext();
};

