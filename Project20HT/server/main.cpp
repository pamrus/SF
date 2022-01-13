
#include <iostream>
#include "ServerToClientConnection.h"


constexpr boost::asio::ip::port_type serverPortNumber = 5190; // The King is dead. Long live the King!

// Несколько глобальных объектов. Пробовал вынести в единый внешний класс - не вышло.
boost::asio::io_service ioService;
boost::asio::ip::tcp::endpoint _listenEndpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), serverPortNumber);		// Endpoint(IPv4, PORT)
boost::asio::ip::tcp::acceptor _listenAcceptor = boost::asio::ip::tcp::acceptor(ioService, _listenEndpoint);						// Acceptor(ioservice, Endpoint)

// Обработка сигнала о подключении клиента.
void onClientConnected(boost::shared_ptr <ServerToClientConnection> client, boost::asio::io_service* _ioService, const boost::system::error_code& err)
{
	client->onStart();	// Новый клиент подключился - запускаем обработку события, ->
	boost::shared_ptr <ServerToClientConnection> new_client(new ServerToClientConnection(_ioService)); // после чего открываем сокет для другого возможного клиента и ->
	_listenAcceptor.async_accept(new_client->getSocket(), boost::bind(onClientConnected, new_client, _ioService, boost::placeholders::_1)); // создаём новый поток для обработки нового возможного подключения
}	

int main()
{
	setlocale(LC_ALL, "");

	boost::shared_ptr <ServerToClientConnection> firstClient(new ServerToClientConnection(&ioService)); // Инициализируем первого клиента ->
	_listenAcceptor.async_accept(firstClient->getSocket(), boost::bind(onClientConnected, firstClient, &ioService, boost::placeholders::_1)); // и запускаем ожидание его подключения 
	ioService.run();

	return 0;
}
