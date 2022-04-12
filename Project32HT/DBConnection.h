#pragma once
#include <string>
#include "gmock/gmock.h"

// Интерфейсный класс DBConnectionInterface для создания дочерних классов, 
// обеспечивающих соединение по различным протоколам, а также для создания мок-класса.
class DBConnectionInterface
{
public:
	DBConnectionInterface() {};
	virtual ~DBConnectionInterface() {};

	virtual bool open(std::string dbName) = 0;
	virtual void close() = 0;
	virtual std::string execQuery(std::string query) = 0;

};

// Реальный класс DBConnection отвечает за низкоуровневое соединение с БД (работа с драйверами, сетью и т.п.). 
// Прототип, не реализован.
class DBConnection : public DBConnectionInterface
{
public:
	DBConnection() {};
	virtual ~DBConnection() {};

	virtual bool open(std::string dbName) override { return true; };
	virtual void close() override {};
	virtual std::string execQuery(std::string query) override { return std::string(); };
};

// Мок-класс MockDBConnection семантически совпадает с классом DBConnection, 
// позволяет при взаимодействии с другими классами осуществлять обмен произвольными данными.
class MockDBConnection : public DBConnectionInterface
{
public:
	MOCK_METHOD(bool, open, (std::string dbName), (override));
	MOCK_METHOD(void, close, (), (override));
	MOCK_METHOD(std::string, execQuery, (std::string query), (override));
};