#pragma once
#include <boost/thread.hpp>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/core/noncopyable.hpp>
#include <iostream>



class ClientToServerConnection;
#define MEM_FN(x)       boost::bind(&ClientToServerConnection::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&ClientToServerConnection::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&ClientToServerConnection::x, shared_from_this(),y,z)

class ClientToServerConnection : public boost::enable_shared_from_this<ClientToServerConnection>// , boost::noncopyable 
{

public:
    ClientToServerConnection(boost::asio::io_service* ioService, const std::string& username) : _ioService(ioService), _socket(*_ioService), _started(true), _login(username), _timer(*_ioService) {}

    void doStart(boost::asio::ip::tcp::endpoint* ep);                   //  
    void doStop();                                                      // Действия при отключении клиента
    
    bool isStarted();

private:
    boost::asio::io_service* _ioService;
    boost::asio::ip::tcp::socket _socket;
    boost::asio::deadline_timer _timer;
    bool _started;
    std::string _login;

    enum { max_msg = 1024 };
    char read_buffer_[max_msg];		// TODO: Заменить на std::string!
    std::string _writeBuffer;

    void onConnect(const boost::system::error_code& err);               // Реакция на успешное подключение - отправка данных аутентификации
    void onRead(const boost::system::error_code& err, size_t bytes);    // Обработка полученных данных после чтения
    void onLogin();                                                     // Реакция на успешный вход
    void onPing(const std::string& msg);                                // Ответ на пинг
    void onClients(const std::string& msg);                             // Вывод полученного списка пользователей
    void onWrite(const boost::system::error_code& err, size_t bytes);   // Действия после каждой отправки данных

    void doRead();                                                      // Чтение данных из сокета
    void doWrite(const std::string& msg);                               // Отправка данных в сокет
    void doPing();                                                      // Отправка пинга
    void doPostponePing();                                              // Запуск ожидания перед отправкой пинга
    void doAskClients();                                                // Отправка запроса на чтение списка пользователей

    size_t readComplete(const boost::system::error_code& err, size_t bytes);    // Проверка завершения чтения

};

