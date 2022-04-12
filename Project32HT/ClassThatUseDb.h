#pragma once
#include "DBConnection.h"
#include <ctime> // Для внесения ошибки в ClassThatUseDb::useConnection при тестировании

// Интерфейсный класс ClassThatUseDbInterface для создания дочерних классов, 
// обеспечивающих различное взаимодействие с БД, а также для создания мок-класса (за рамками данного ТЗ).
class ClassThatUseDbInterface
{
public:
	ClassThatUseDbInterface() {};
	ClassThatUseDbInterface(DBConnectionInterface* dbHandle) {};
	virtual ~ClassThatUseDbInterface() {}

	virtual void openConnection(std::string dbName) = 0;
	virtual std::string useConnection(std::string query) = 0;
	virtual void closeConnection() = 0;

protected:
	DBConnectionInterface* _dbConnection = nullptr;
	bool _connectionEstablished = false;
};

// Реальный класс ClassThatUseDb отвечает за высокоуровневое взаимодействие с БД. 
// Прототип, реализовано базовое взаимодействие с низким уровнем (DBConnectionInterface). 
// Для этих методов созданы тесты, в которых реальный объект низкого уровня имитируется мок-объектом MockDBConnection.
class ClassThatUseDb : public ClassThatUseDbInterface
{
public:
	ClassThatUseDb(DBConnectionInterface* dbHandle); // Конструктор, принимающий внешне-созданный объект DBConnectionInterface в качестве хэндла _dbConnection
	virtual ~ClassThatUseDb() {};

	void openConnection(std::string dbName) override;
	std::string useConnection(std::string query) override;
	void closeConnection() override;
};
