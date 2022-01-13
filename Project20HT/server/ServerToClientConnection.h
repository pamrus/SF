#pragma once

#include <iostream>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>


class ServerToClientConnection;
typedef std::vector< boost::shared_ptr< ServerToClientConnection > > clientsArrayType;	// Определение типа для хранения списка подключенных клиентов
clientsArrayType clientsArray; // Глобальная переменная для хранения полного списка клиентов, с которыми в конкретный момент времени установлена связь 
//TODO: добавить семафор на одновременный доступ из ::onStart, ::onStop, ::onClients

void updateClientsChanged(); // Принудительная установка флага _clientsChanged у всех клиентов
#define MEM_FN(x)       boost::bind(&ServerToClientConnection::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&ServerToClientConnection::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&ServerToClientConnection::x, shared_from_this(),y,z)

class ServerToClientConnection : public boost::enable_shared_from_this<ServerToClientConnection> //, boost::noncopyable
{
public:
	ServerToClientConnection(boost::asio::io_service* ioService);

	void onStart();														// Действия после подключения клиента к порту
	void onStop();														// Действия при отключении клиента

	bool							isStarted() const;
	boost::asio::ip::tcp::socket&	getSocket();
	std::string						getLogin() const;
	void							setClientsChanged();

private:
	boost::asio::io_service* _ioService;
	boost::asio::ip::tcp::socket _socket;
	boost::asio::deadline_timer _timer;
	boost::posix_time::ptime _lastPing;
	bool _started;
	bool _clientsChanged;
	std::string _login;

	enum { max_msg = 1024 };
	char read_buffer_[max_msg];		// TODO: Заменить на std::string!
	std::string _writeBuffer;

	void onRead(const boost::system::error_code& err, size_t bytes);	// Обработка полученных данных после чтения
	void onWrite(const boost::system::error_code& err, size_t bytes);	// Действия после каждой отправки данных
	void onLogin(const std::string& msg);								// Реакция на запрос о входе в чат TODO: проверка логина и пароля по базе пользователей
	void onPing();														// Ответ на пинг
	void onClients();													// Реакция на запрос о списке онлайн-пользователей
	void onCheckPing();													// Запускающаяся по таймеру проверка онлайн-статуса клиента

	void doRead();														// Чтение данных из сокета
	void doWrite(const std::string& msg);								// Отправка данных в сокет
	
	size_t readComplete(const boost::system::error_code& err, size_t bytes); // Проверка завершения чтения
};

