#include "ClassThatUseDb.h"

ClassThatUseDb::ClassThatUseDb(DBConnectionInterface* dbHandle)
{
	_dbConnection = dbHandle;
}

void ClassThatUseDb::openConnection(std::string dbName)
{
	if(!dbName.empty())
		if (_dbConnection->open(dbName))
			_connectionEstablished = true;
}

std::string ClassThatUseDb::useConnection(std::string query)
{
	if (_connectionEstablished)
	{
		srand((unsigned)time(0));
		if (rand() % 2)
			return "DB wrong reply";	// Симуляция ошибки с вероятностью ~0.5: 
		//EXPECT_EQ перехватит результат, EXPECT_CALL перехватит "невызов" execQuery

		return _dbConnection->execQuery(query);	// Отправляем запрос на нижний уровень
	}
	else
		return std::string();
}

void ClassThatUseDb::closeConnection()
{
	if (_connectionEstablished)
		_dbConnection->close();
}
